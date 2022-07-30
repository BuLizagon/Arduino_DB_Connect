<?php

header("Content-Type: text/html;charset=UTF-8");
 
$db_user = ""; //데이터베이스 아이디

$db_passwd = "";     //데이터베이스 비밀번호

$db_name = ""; //데이터베이스 이름 

$mysqli = new mysqli("localhost", $db_user, $db_passwd, $db_name);


if($mysqli){
    
    echo "데이터베이스 연결 성공";
    
    $p_sensor=$_GET['p_sensor'];  //포토 다이오드 센서
    $l_sensor=$_GET['l_sensor'];  //적외선 센서
    
    //데이터베이스 삽입문
    if($p_sensor==1 and $l_sensor==0){
        
        $query = "INSERT INTO all_sensor(time, p, l) VALUES (now(), '감지', '감지')";
        
        mysqli_query($mysqli, $query);
    }
    
}
else{
    echo "데이터베이스 연결 실패";
}
//종료
mysqli_close($mysqli);
?>