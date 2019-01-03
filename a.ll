; ModuleID = 'alan program'

@vars = private global [26 x i32] zeroinitializer, align 16
@nl = private constant [2 x i8] c"\0A\00", align 1

declare void @writeInteger(i16)

declare void @writeChar(i8)

declare void @writeString(i8*)

declare i8 @readInteger()

define void @main() {
entry:
  %g = alloca i8
  %y = alloca i8
  %x = alloca i16
  store i8 97, i8* %g
  store i8 98, i8* %y
  %string = alloca [16 x i8]
  %str = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 0
  store i8 65, i8* %str
  %str1 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 1
  store i8 70, i8* %str1
  %str2 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 2
  store i8 84, i8* %str2
  %str3 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 3
  store i8 65, i8* %str3
  %str4 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 4
  store i8 65, i8* %str4
  %str5 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 5
  store i8 65, i8* %str5
  %str6 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 6
  store i8 65, i8* %str6
  %str7 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 7
  store i8 32, i8* %str7
  %str8 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 8
  store i8 69, i8* %str8
  %str9 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 9
  store i8 73, i8* %str9
  %str10 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 10
  store i8 78, i8* %str10
  %str11 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 11
  store i8 65, i8* %str11
  %str12 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 12
  store i8 73, i8* %str12
  %str13 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 13
  store i8 73, i8* %str13
  %str14 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 14
  store i8 33, i8* %str14
  %str15 = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 15
  store i8 0, i8* %str15
  %str_ptr = getelementptr inbounds [16 x i8], [16 x i8]* %string, i8 0, i8 0
  call void @writeString(i8* %str_ptr)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  call void @writeInteger(i16 2)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  store i16 2, i16* %x
  %y16 = load i8, i8* %y
  call void @writeChar(i8 %y16)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  call void @writeInteger(i16 97)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  call void @writeInteger(i16 -1)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  ret void
}
