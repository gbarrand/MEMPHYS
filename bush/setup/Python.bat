@ECHO off 

FOR /F "usebackq delims=;" %%d IN (`CD`) DO SET save_dir=%%d

CD ..\..\inexlib\ourex\Python

FOR /F "usebackq delims=;" %%d IN (`CD`) DO SET Python_home=%%d

SET PYTHONHOME=%Python_home%

CD /D %save_dir%

SET save_dir=

@ECHO on
