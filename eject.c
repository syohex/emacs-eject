/* Eject CD tray

Copyright 2015 Syohei YOSHIDA

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.  */

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/cdrom.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <emacs-module.h>

int plugin_is_GPL_compatible;

static emacs_value
Feject(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	int fd = open("/dev/cdrom", O_RDONLY|O_NONBLOCK);
	if (fd < 0) {
		perror("open /dev/cdrom");
		return env->intern(env, "nil");
	}

	int cmd = CDROMEJECT;
	if (!env->is_not_nil(env, args[0])) {
		if (ioctl(fd, CDROM_DRIVE_STATUS, 0) == CDS_TRAY_OPEN)
			cmd = CDROMCLOSETRAY;
		else
			cmd = CDROMEJECT;
	}

	if (ioctl(fd, cmd, 0) < 0) {
		perror("ioctl");
		return env->intern(env, "nil");
	}

	return env->intern(env, "t");
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

	emacs_value ejectfn = env->make_function(env, 0, 1, Feject, "Control CD tray", NULL);

	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qeject = env->intern(env, "eject");
	emacs_value fset_args[] = { Qeject, ejectfn };
	env->funcall(env, Qfset, 2, fset_args);

	emacs_value Qprovide = env->intern(env, "provide");
	emacs_value provide_args[] = { Qeject };
	env->funcall(env, Qprovide, 1, provide_args);

	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
