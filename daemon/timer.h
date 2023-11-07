#include <iostream>
#include <functional>
#include <csignal>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif


class Timer {

public:

#ifdef __linux__
    static Timer* timer_instance;
#endif
#ifdef __linux__
    Timer() : is_running(false), callback(nullptr), user_data(nullptr) {
        signal_number = SIGALRM;
        timer_instance = this;
    }
#elif _WIN32
Timer() : timer_queue(nullptr), timer(nullptr), callback(nullptr), user_data(nullptr) {}
#endif

    // Set the callback function and user data (optional)
    void SetCallback(std::function<void(void*)> callback_func, void* data = nullptr) {
        callback = callback_func;
        user_data = data;

    }

    void Start([[maybe_unused]]int milliseconds) {
        if (is_running) {
            std::cerr << "Timer is already running." << std::endl;
            return;
        }

#ifdef _WIN32
        timer_queue = CreateTimerQueue();
        if (timer_queue != nullptr) {
            // Create a timer queue

            // Create a timer in the timer queue
            if (!CreateTimerQueueTimer( &timer, timer_queue,
                                        (WAITORTIMERCALLBACK)TimerCallback, this , milliseconds, milliseconds, 0)) {
                std::cerr << "Failed to create timer." << std::endl;
                return;
            }
        }
        else {
            if (NULL == timer_queue) {
                std::cerr << "Failed to create timer queue." << std::endl;
                return;
            }
        }
#else
        struct itimerval timer_spec;

        timer_spec.it_value.tv_sec = milliseconds / 1000;
        timer_spec.it_value.tv_usec = 0;
        timer_spec.it_interval.tv_sec = milliseconds / 1000;
        timer_spec.it_interval.tv_usec = 0;

        setitimer(ITIMER_REAL, reinterpret_cast<const itimerval *>(&timer_spec), nullptr);
        signal(SIGALRM, TimerSignalHandlerWrapper);
#endif
        is_running = true;
    }

    void Stop() {
#ifdef __linux__
        if (!is_running) {
            return;
        }

        struct itimerval timer_spec;
        memset(&timer_spec, 0, sizeof(timer_spec));
        if (setitimer(ITIMER_REAL, reinterpret_cast<const itimerval *>(&timer_spec), nullptr) == -1) {
            std::cerr << "Failed to stop timer." << std::endl;
        }
#elif _WIN32
        if (timer_queue != nullptr) {
            // Delete the timer and the timer queue
            if (DeleteTimerQueueTimer(timer_queue, timer, nullptr)) {
                DeleteTimerQueue(timer_queue);
                timer_queue = nullptr;
                timer = nullptr;
            } else {
                std::cerr << "Failed to delete timer." << std::endl;
            }
        }
#endif
        is_running = false;
    }

    bool IsRunning() const {
        return is_running;
    }

private:
#ifdef _WIN32
    UINT_PTR timer_id;
    HANDLE timer_queue;
    HANDLE timer;
#endif
    bool is_running;
    std::function<void(void*)> callback;
    void* user_data;
    int signal_number;

#ifdef _WIN32
    static VOID CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
        if (lpParameter == NULL) {
            printf("TimerRoutine lpParam is NULL\n");
        } else {
            Timer *timer_instance = static_cast<Timer *>(lpParameter);
            if (timer_instance->callback) {
                timer_instance->callback(timer_instance->user_data);
            }
        }
    }
#else
    void TimerSignalHandler([[maybe_unused]]int signum) {
        // Handle the timer signal here
        // Call the callback function
        if (callback) {
            callback(user_data);
        }
    }

    static void TimerSignalHandlerWrapper(int signum) {
        if (timer_instance && signum == timer_instance->signal_number) {
            timer_instance->TimerSignalHandler(signum);
        }
    }
#endif
};

