REM VB.Net program to calculate the MD5 of Expert Witness Compression Format file(s) using ewf.net
REM
REM Author:            Joachim Metz
REM Creation date:     November 3, 2010
REM Modification date: October 16, 2011

Imports MD5net
Imports EWF

Module MD5sum

Function BinaryToHexString(ByVal data() As Byte) As String
    Dim string_builder As New System.Text.StringBuilder( data.Length() * 2 )

    For Each value As Byte In data
        string_builder.Append(value.ToString("X2"))
    Next

    Return string_builder.ToString().ToLower()
End Function

Function Main(ByVal args() As String) As Integer
	Dim md5_context As New MD5net.Context
	Dim handle As New EWF.Handle

	Dim filenames As Array    = Nothing
	Dim md5_hash() As Byte    = Nothing
	Dim program As String     = "MD5sum.exe"
	Dim media_size As UInt64  = 0
	Dim read_count As Integer = 0
	Dim read_size As Integer  = 0

	Dim buffer(4096) As Byte

	Console.WriteLine(program + " (ewf.net " + EWF.Support.GetVersion() + ")")

	If args.Length() < 1 Then
		Console.WriteLine("Usage: " + program + " filename")

		Return 1
	End If

	If args.Length() = 1 Then
		filenames = EWF.Handle.Glob(args(0))
	Else
		filenames = args
	End If

	If filenames.Length() <= 0 Then
		Console.WriteLine("Unable to determine filename(s)")

		Return 1
	End If

	handle.Open(filenames, EWF.Handle.GetAccessFlagsRead())

	media_size = handle.GetMediaSize()

	While media_size > 0
		If media_size > 4096 Then
			read_size = 4096
		Else
			read_size = media_size
		End If

		read_count = handle.ReadBuffer(buffer, read_size)

		If read_count <> read_size Then
			Console.WriteLine("Unable to read from files")

			Return 1
		End If

		md5_context.Update(buffer, read_size)

		media_size -= read_size
	End While

	handle.Close()

	md5_hash = md5_context.GetHash()

    Console.WriteLine("MD5: " + BinaryToHexString(md5_hash))

	Console.WriteLine("")

	Return 0
End Function

End Module

