#!../wsbasic

begin

  #change the dir so that is the same as in the makefile!
  INSTALL_DIR ="/usr/local/bin"
  NEWPATH     ="#!"+INSTALL_DIR+"/wsbasic"

  foreach file in run( "ls -1" )

    if file != "changepath.b" 
      script= run( "cat "+file+" | grep -v /wsbasic " )
      script= NEWPATH+"\n"+script
      write( file, script )
    end
    
  end
end

