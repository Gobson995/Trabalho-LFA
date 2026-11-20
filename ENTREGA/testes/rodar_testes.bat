@echo off
cd ..\src

echo Devem ser aceitos:
for %%f in (..\testes\validos\*.pas) do (
    main.exe "%%f" > nul
    if errorlevel 1 (echo   %%f  ERRO) else (echo   %%f  ok)
)

echo Devem ser rejeitados:
for %%f in (..\testes\invalidos\*.pas) do (
    main.exe "%%f" > nul
    if errorlevel 1 (echo   %%f  ok) else (echo   %%f  ERRO)
)

pause
