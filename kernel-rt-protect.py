# SPDX-License-Identifier: GPL-2.0-or-later
from yum.plugins import TYPE_INTERACTIVE
requires_api_version = '2.5'
plugin_type = (TYPE_INTERACTIVE,)

def config_hook(conduit):
    def get_running_kernel_pkgtup(ts):
        """This takes the output of uname and figures out the pkgtup of the running
           kernel (name, arch, epoch, version, release)."""
        import os, glob
        ver = os.uname()[2]

        # we glob for the file that MIGHT have this kernel
        # and then look up the file in our rpmdb.
        fns = sorted(glob.glob('/boot/vmlinuz*%s*' % ver))
        for fn in fns:
            mi = ts.dbMatch('basenames', fn)
            for h in mi:
                e = h['epoch']
                if h['epoch'] is None:
                    e = '0'
                else:
                    e = str(e)
                return (h['name'], h['arch'], e, h['version'], h['release'])
        return (None, None, None, None, None)

    from yum import misc
    misc.get_running_kernel_pkgtup = get_running_kernel_pkgtup
