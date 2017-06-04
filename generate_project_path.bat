@echo off

set QTDIR=D:\Qt\Qt5.5.1\5.5\msvc2010
set boost_dir=%cd%\depend\boost
set libcurl_dir=%cd%\depend\libcurl
set OpenSSL_dir=%cd%\depend\OpenSSL

set path=%QTDIR%\bin;%boost_dir%\bin;%libcurl_dir%\bin;%OpenSSL_dir%\bin;%path%

cmd.exe

