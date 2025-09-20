@echo off
echo Building firmware...
platformio run --environment esp32dev
if %errorlevel% neq 0 (
  echo Build failed.
  pause
  exit /b %errorlevel%
)

echo Uploading firmware...
platformio run --target upload --environment esp32dev
if %errorlevel% neq 0 (
  echo Upload failed.
  pause
  exit /b %errorlevel%
)

echo Building filesystem image...
platformio run --target buildfs --environment esp32dev
if %errorlevel% neq 0 (
  echo Build filesystem failed.
  pause
  exit /b %errorlevel%
)

echo Uploading filesystem image...
platformio run --target uploadfs --environment esp32dev
if %errorlevel% neq 0 (
  echo Upload filesystem failed.
  pause
  exit /b %errorlevel%
)

echo Opening serial monitor (115200)...
start "" cmd /k "platformio device monitor --environment esp32dev --baud 115200"

echo Done.
pause