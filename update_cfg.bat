set CHIBISTUDIO=C:\ChibiStudio
set PATH=%CHIBISTUDIO%\tools\msys2\usr\bin;%PATH%
set PATH=%CHIBISTUDIO%\tools\openocd\bin;%PATH%
set PATH=%CHIBISTUDIO%\tools\fmpp\bin;%PATH%
set JAVA_HOME=%CHIBISTUDIO%\tools\jdk
set CH_VER=_trunk

CD C:\ChibiStudio\chibios%CH_VER%\tools\updater

bash update_chconf_rt.sh rootpath ../../../playembedded%CH_VER%/
bash update_chconf_nil.sh rootpath ../../../playembedded%CH_VER%/
bash update_halconf.sh rootpath ../../../playembedded%CH_VER%/
bash update_mcuconf_stm32f401xx.sh rootpath ../../../playembedded%CH_VER%/
bash update_mcuconf_stm32f303xx.sh rootpath ../../../playembedded%CH_VER%/
pause