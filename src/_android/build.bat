call config.bat
if exist %PROJECT_DIR%\obj rd /s /q %PROJECT_DIR%\obj
ndk-build TARGET_PLATFORM=android-9