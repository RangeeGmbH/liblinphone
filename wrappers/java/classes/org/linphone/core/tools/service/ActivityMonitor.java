/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone 
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
package org.linphone.core.tools.service;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;

import org.linphone.core.tools.AndroidPlatformHelper;
import org.linphone.core.tools.Log;

import java.util.ArrayList;

/**
 * Believe me or not, but knowing the application visibility state on Android is a nightmare. After
 * two days of hard work I ended with the following class, that does the job more or less reliabily.
 */
public class ActivityMonitor implements Application.ActivityLifecycleCallbacks {
    private final ArrayList<Activity> activities = new ArrayList<>();
    private boolean mActive = false;
    private int mRunningActivities = 0;
    private InactivityChecker mLastChecker;

    @Override
    public synchronized void onActivityCreated(Activity activity, Bundle savedInstanceState) {
        Log.i("[Activity Monitor] Activity created:" + activity);
        if (!activities.contains(activity)) activities.add(activity);
    }

    @Override
    public void onActivityStarted(Activity activity) {
        Log.i("Activity started:" + activity);
    }

    @Override
    public synchronized void onActivityResumed(Activity activity) {
        Log.i("[Activity Monitor] Activity resumed:" + activity);
        if (!activities.contains(activity)) {
            Log.w("[Activity Monitor] Activity wasn't registered yet...");
            activities.add(activity);
        }

        mRunningActivities++;
        Log.i("[Activity Monitor] runningActivities=" + mRunningActivities);
        checkActivity();
    }

    @Override
    public synchronized void onActivityPaused(Activity activity) {
        Log.i("[Activity Monitor] Activity paused:" + activity);
        if (!activities.contains(activity)) {
            Log.w("[Activity Monitor] Activity wasn't registered yet...");
            activities.add(activity);
        } else {
            mRunningActivities--;
            Log.i("[Activity Monitor] runningActivities=" + mRunningActivities);
            checkActivity();
        }
    }

    @Override
    public void onActivityStopped(Activity activity) {
        Log.i("[Activity Monitor] Activity stopped:" + activity);
    }

    @Override
    public synchronized void onActivityDestroyed(Activity activity) {
        Log.i("[Activity Monitor] Activity destroyed:" + activity);
        activities.remove(activity);
    }

    void startInactivityChecker() {
        if (mLastChecker != null) mLastChecker.cancel();
        AndroidDispatcher.dispatchOnUIThreadAfter((mLastChecker = new InactivityChecker()), 2000);
    }

    void checkActivity() {
        if (mRunningActivities == 0) {
            if (mActive) startInactivityChecker();
        } else if (mRunningActivities > 0) {
            if (!mActive) {
                mActive = true;
                onForegroundMode();
            }
            if (mLastChecker != null) {
                mLastChecker.cancel();
                mLastChecker = null;
            }
        }
    }

    private void onBackgroundMode() {
        Log.i("[Activity Monitor] App has entered background mode");
        if (AndroidPlatformHelper.isReady()) {
            AndroidPlatformHelper.instance().onBackgroundMode();
        }
    }

    private void onForegroundMode() {
        Log.i("[Activity Monitor] App has left background mode");
        if (AndroidPlatformHelper.isReady()) {
            AndroidPlatformHelper.instance().onForegroundMode();
        }
    }

    @Override
    public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
    }

    class InactivityChecker implements Runnable {
        private boolean isCanceled;

        void cancel() {
            isCanceled = true;
        }

        @Override
        public void run() {
            if (AndroidPlatformHelper.isReady()) {
                synchronized (AndroidPlatformHelper.instance()) {
                    if (!isCanceled) {
                        if (ActivityMonitor.this.mRunningActivities == 0 && mActive) {
                            mActive = false;
                            onBackgroundMode();
                        }
                    }
                }
            }
        }
    }
}
