using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

class Program
{
    static int Main(string[] args)
    {
        string toolDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)
                         ?? AppContext.BaseDirectory;
        string binaryName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ? "halixAssembler.exe" : "halixAssembler";
        string binaryPath = Path.Combine(toolDir, binaryName);
        if (!File.Exists(binaryPath)) binaryPath = binaryName;
        string arguments = string.Join(" ", args);
        var process = new Process
        {
            StartInfo = new ProcessStartInfo
            {
                FileName = binaryPath,
                Arguments = arguments,
                UseShellExecute = false,
            }
        };
        try
        {
            process.Start();
            process.WaitForExit();
            return process.ExitCode;
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"ERROR: Could not run halixAssembler: {ex.Message}");
            return 1;
        }
    }
}
