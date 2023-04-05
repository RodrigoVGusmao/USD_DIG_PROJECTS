<# :
:: Based on https://gist.github.com/coldnebo/1148334
:: Converted to a batch/powershell hybrid via http://www.dostips.com/forum/viewtopic.php?p=37780#p37780
@echo off
setlocal
set "POWERSHELL_BAT_ARGS=%*"
if defined POWERSHELL_BAT_ARGS set "POWERSHELL_BAT_ARGS=%POWERSHELL_BAT_ARGS:"=\"%"
endlocal & powershell -NoLogo -NoProfile -Command "$_ = $input; Invoke-Expression $( '$input = $_; $_ = \"\"; $args = @( &{ $args } %POWERSHELL_BAT_ARGS% );' + [String]::Join( [char]10, $( Get-Content \"%~f0\" ) ) )"
goto :EOF
#>

Add-Type @"
  using System;
  using System.Runtime.InteropServices;

public class Win32 {
    [DllImport("user32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
  }
"@

$progname = $args[4]
$progargs = $args[5..($args.Count-1)]
#Write-Host "progargs=$progargs"
$MyProcess = Start-Process -FilePath $progname -ArgumentList $progargs -PassThru

While ($MyProcess.MainWindowHandle -eq 0) {
Start-Sleep -Seconds 1
}

$h = $MyProcess.MainWindowHandle

$screen_x=$args[0]
$screen_y=$args[1]
$win_width=$args[2]
$win_height=$args[3]

# Set MoveWindow output = to $ret so that it doesn't print 'True' to the terminal
$ret = [Win32]::MoveWindow($h, $screen_x, $screen_y, $win_width + $screen_x, $win_height + $screen_y, $true )