javac -cp jssc.jar jarduino\*.java
javac -cp jssc.jar;%CD% JSTest.java
pause
java -cp jssc.jar;%CD% JSTest