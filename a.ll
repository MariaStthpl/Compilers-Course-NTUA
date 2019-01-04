; ModuleID = 'alan program'

@vars = private global [26 x i32] zeroinitializer, align 16
@nl = private constant [2 x i8] c"\0A\00", align 1

declare void @writeInteger(i16)

declare i8 @ord(i8)

declare void @writeChar(i8)

declare void @writeString(i8*)

declare i16 @readInteger()

declare i8 @readChar()

declare void @readString(i16, i8*)

declare i16 @strlen(i8*)

declare i16 @strcmp(i8*, i8*)

declare void @strcpy(i8*, i8*)

declare void @strcat(i8*, i8*)

define void @main() {
entry:
  %string = alloca [8 x i8]
  %str = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 0
  store i8 100, i8* %str
  %str1 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 1
  store i8 97, i8* %str1
  %str2 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 2
  store i8 100, i8* %str2
  %str3 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 3
  store i8 97, i8* %str3
  %str4 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 4
  store i8 115, i8* %str4
  %str5 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 5
  store i8 92, i8* %str5
  %str6 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 6
  store i8 110, i8* %str6
  %str7 = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 7
  store i8 0, i8* %str7
  %str_ptr = getelementptr inbounds [8 x i8], [8 x i8]* %string, i8 0, i8 0
  call void @writeString(i8* %str_ptr)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @nl, i32 0, i32 0))
  ret void
}
