REM VB.Net program to open and close (split) Expert Witness Compression Format file(s) using ewf.net
REM
REM Author:            Joachim Metz
REM Creation date:     November 3, 2010
REM Modification date: October 16, 2011

Imports EWF

Module OpenClose

Function Main(ByVal args() As String) As Integer
        Dim handle As New EWF.Handle 

	Dim program As String = "OpenClose.exe"

	Console.WriteLine(program + " (ewf.net " + EWF.Support.GetVersion() + ")")

	If args.Length() < 1 Then
		Console.WriteLine("Usage: " + program + " filename(s)")

		Return 1
	End If

	handle.Open(args, EWF.Handle.GetAccessFlagsRead())

	handle.Close()

	Console.WriteLine("")

	Return 0
End Function

End Module

