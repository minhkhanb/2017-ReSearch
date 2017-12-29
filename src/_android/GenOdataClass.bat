call config.bat
set LIBS=./
set LIBS=%LIBS%;%RESTLET_LIB%
set LIBS=%LIBS%;%RESTLET_LIB%/org.restlet.jar
set LIBS=%LIBS%;%RESTLET_LIB%/org.restlet.ext.odata.jar
set LIBS=%LIBS%;%RESTLET_LIB%/org.restlet.ext.freemarker.jar
set LIBS=%LIBS%;%RESTLET_LIB%/org.restlet.ext.atom.jar
set LIBS=%LIBS%;%RESTLET_LIB%/org.restlet.ext.xml.jar
set LIBS=%LIBS%;%FREE_MAKER_LIB%/freemarker.jar

java -cp ./tools;%LIBS% POJOGenerator %ODATA_SVC_URL% ./src %ODATA_SVC_NAMESPACE%
pause