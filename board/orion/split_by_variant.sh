#!/bin/bash
# ---------------------------------------------------------
#  Set the core module defines according to Core Module
# ---------------------------------------------------------
# ---------------------------------------------------------
# Set up the Versatile type define
# ---------------------------------------------------------

strIsSubstring()
{
  local x=0
  if [ "$(echo $1 | grep $2)" == "" ]
      then
      x=1
  fi  
  return $x
}

strIsSubstring2()
{
  local x=0
  if [ "$(echo $1 | grep $2)" == "" ]
  then
      if [ "$(echo $1 | grep $3)" == "" ]
      then
          x=1
      fi
  fi  
  return $x
}

mkdir -p ${obj}include
variant=CSM1200

if [ "$1" == "" ]
then
	echo "$0:: No parameters - using orion14_config"
	echo "#define CONFIG_ARCH_CSM1200" > ${obj}include/config.h
else
    if  strIsSubstring2 "$1" "orion13_config" "csm1100_config"
    then
        echo "#define CONFIG_ARCH_CSM1100" > ${obj}include/config.h
        variant=CSM1 100
    else
        if  strIsSubstring2 "$1" "orion14" "csm1200"
        then
            echo "#define CONFIG_ARCH_CSM1200" > ${obj}include/config.h    
            echo "... configured for csm1200"
            
            if  strIsSubstring "$1"  "overload"
            then
                echo "#define CONFIG_CSM1200_OVERLOAD" >> ${obj}include/config.h
                echo "... configured for csm1200 overload"
            fi
            
            
            variant=CSM1200
        else      
        	if strIsSubstring "$1" "mrhgold"
        	then
	            echo "#define CONFIG_ARCH_CSM1201" > ${obj}include/config.h
	            echo "#define MERIH_GOLD_BOARD" >> ${obj}include/config.h 
        			echo "... configured for Merih Gold"
        			variant=CSM1201
	            if  strIsSubstring "$1"  "32mflashmode"  
	            then 
	            	echo "#define CONFIG_32M_FLASHMODE" >> ${obj}include/config.h 
	            	echo "... configured for csm1201 to 32M Flash mode"
	           	else
	           		if strIsSubstring "$1"  "16mflashmode"
	           		then
	            		echo "#define CONFIG_16M_FLASHMODE" >> ${obj}include/config.h 
	            		echo "... configured for csm1201 to 16M Flash mode"
	           		fi
	            fi
        	else
            if strIsSubstring "$1" "csm1201"
            then
                if strIsSubstring "$1" "csm1201j"
                then
                    echo "#define CONFIG_ARCH_CSM1201" > ${obj}include/config.h
                    echo "#define CONFIG_16BIT_DDR" >> ${obj}include/config.h 
                    echo "... configured for csm1201j"
                    if strIsSubstring "$1" "128m"
                    then
                        echo "#define CONFIG_256M_DDR" >> ${obj}include/config.h
                        echo "#define MAX_SDRAM  0x10000000" >> ${obj}include/config.h
                        echo "... configured 128M DDR for csm1201j"
                    fi
                    variant=CSM1201
                else
                    echo "#define CONFIG_ARCH_CSM1201" > ${obj}include/config.h
                    echo "... configured for csm1201"
                    variant=CSM1201
                fi
                if  strIsSubstring "$1"  "32mflashmode"  
                then 
                    echo "#define CONFIG_32M_FLASHMODE" >> ${obj}include/config.h 
                    echo "... configured for csm1201 to 32M Flash mode"
                fi
            fi
        	fi
        fi
        if  strIsSubstring "$1" "nand"  
        then 
            echo "#define CONFIG_ARCH_CSM_NAND_BOOT" >> ${obj}include/config.h 
            echo "... configured for nand flash boot"
        fi
        
        if  strIsSubstring2 "$1"  "16bit" "csm1200j"  
        then    
            echo "#define CONFIG_16BIT_DDR" >> ${obj}include/config.h 
            echo "... configured for 16bit interface of DDR"
        fi

        if  strIsSubstring "$1"  "256m"  
            then 
            echo "#define CONFIG_256M_DDR" >> ${obj}include/config.h 
            echo "#define MAX_SDRAM  0x10000000" >> ${obj}include/config.h 
            echo "... configured for 256M DDR"
        fi
        if  strIsSubstring "$1"  "sst"  
            then 
            echo "#define CONFIG_SST_FLASH" >> ${obj}include/config.h 
            echo "... configured for SST Nor Flash"
        fi

        if  strIsSubstring "$1" "spi"  
        then 
            echo "#define CONFIG_ARCH_CSM_SPI_BOOT" >> ${obj}include/config.h 
            echo "... configured for spi flash boot"
        fi
 
    fi


fi


# ---------------------------------------------------------
# Complete the configuration
# ---------------------------------------------------------
$MKCONFIG -a orion arm arm926ejs orion NULL orion
if [ "$variant" == "CSM1201" ]; then echo "UseSecureBoot=yes" >> ${obj}include/config.mk; fi
echo "Variant:: $variant"
