import sys
import gdb

# Update module path.
dir_ = '/home/me/output/wyze_c3_t31x_gc2053_rtl8189ftv_ffmpeg/per-package/opencv4/host/share/glib-2.0/gdb'
if not dir_ in sys.path:
    sys.path.insert(0, dir_)

from gobject_gdb import register
register (gdb.current_objfile ())
