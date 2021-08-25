
# to run X11 apps from Windows-10/WSL.

# VcXsrv can be downloaded from source forge. At startup choose the config:
#   multiple windows
#   display 0
#   start no client
#   disable native opengl (sic).

export DISPLAY=localhost:0
