@echo off
setlocal EnableDelayedExpansion

:: ==============================================================================
:: 
::      ｂｕｉｌｄ．ｂａｔ
::
::      𝒸𝓊𝓈𝓉ℴ𝓂 𝒷𝓊𝒾𝓁𝒹 𝓈𝒸𝓇𝒾𝓅𝓉 𝒻ℴ𝓇 ℊℯ𝓉𝒶𝒹𝓂
::
:: ==============================================================================
::   arsccriptum - made in quebec 2020 <guillaumeplante.qc@gmail.com>
:: ==============================================================================

goto :init

:init
    set "__scripts_root=%AutomationScriptsRoot%"
    call :read_script_root development\build-automation  BuildAutomation
    set "__script_file=%~0"
    set "__target=%~1"
    set "__script_path=%~dp0"
    set "__makefile=%__scripts_root%\make\make.bat"
    set "__lib_date=%__scripts_root%\batlibs\date.bat"
    set "__lib_out=%__scripts_root%\batlibs\out.bat"
    ::*** This is the important line ***
    set "__build_cfg=%__script_path%buildcfg.ini"
    set "__build_cancelled=0"
    goto :validate


:header
    echo. %__script_name% v%__script_version%
    echo.    This script is part of codecastor build wrappers.
    echo.
    goto :eof

:header_err
    echo.**************************************************
    echo.This script is part of codecastor build wrappers.
    echo.**************************************************
    echo.
    echo. YOU NEED TO HAVE THE BuildAutomation Scripts setup on you system...
    echo. https://github.com/codecastor/BuildAutomation
    goto :eof


:read_script_root
    set regpath=%OrganizationHKCU::=%
    for /f "tokens=2,*" %%A in ('REG.exe query %regpath%\%1 /v %2') do (
            set "__scripts_root=%%B"
        )
    goto :eof

:validate
    if not defined __scripts_root          call :header_err && call :error_missing_path __scripts_root & goto :eof
    if not exist %__makefile%  call :error_missing_script "%__makefile%" & goto :eof
    if not exist %__lib_date%  call :error_missing_script "%__lib_date%" & goto :eof
    if not exist %__lib_out%  call :error_missing_script "%__lib_out%" & goto :eof
    if not exist %__build_cfg%  call :error_missing_script "%__build_cfg%" & goto :eof
    goto :build


:: ==============================================================================
::   call make
:: ==============================================================================
:call_make_build
    set config=%1
    set platform=%2
    call %__lib_date% :getbuilddate
    call %__lib_out% :__out_l_blu "=============================================================================="
    call %__lib_out% :__out_l_cya "build start - %__build_cfg% - %config% - %platform%"
    call %__lib_out% :__out_l_blu "=============================================================================="    
    call %__makefile% /v /i %__build_cfg% /t Build /c %config% /p %platform%
    goto :finished



:: ==============================================================================
::   Build x86
:: ==============================================================================
:build_x86
    call :call_make_build Debug x86
    call :call_make_build Release x86
    goto :eof


:: ==============================================================================
::   clean all
:: ==============================================================================
:clean
    call %__makefile% /v /i %__build_cfg% /t Clean /c Debug /p x86
    call %__makefile% /v /i %__build_cfg% /t Clean /c Release /p x86
    call %__makefile% /v /i %__build_cfg% /t Clean /c Debug /p x64
    call %__makefile% /v /i %__build_cfg% /t Clean /c Release /p x64
    goto :eof



:: ==============================================================================
::   Build
:: ==============================================================================
:build
    call :build_x86
    goto :finished


:finished
    ::call %__lib_out% :__out_l_cya "Finished"
    goto :eof
