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

#include "core-accessor.h"

#include "logger/logger.h"

// =============================================================================

using namespace std;

LINPHONE_BEGIN_NAMESPACE

class CoreAccessorPrivate {
public:
	weak_ptr<Core> core;
};

// -----------------------------------------------------------------------------

CoreAccessor::CoreAccessor(const shared_ptr<Core> &core) {
	mPrivate = new CoreAccessorPrivate();
	if (core) mPrivate->core = core;
}

CoreAccessor::~CoreAccessor() {
	delete mPrivate;
}

shared_ptr<Core> CoreAccessor::getCore() const {
	L_D();

	shared_ptr<Core> core = d->core.lock();
	if (!core) {
		lWarning() << "Unable to get valid core instance.";
		throw bad_weak_ptr();
	}

	return core;
}

void CoreAccessor::setCore(std::shared_ptr<Core> core) {
	L_D();
	d->core = core;
}

LINPHONE_END_NAMESPACE
