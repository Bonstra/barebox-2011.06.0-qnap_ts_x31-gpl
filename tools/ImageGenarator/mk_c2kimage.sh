#!/bin/sh
################################################################################
# This script generates c2k images(that includes image header) from the standard
# binary file. It can generates header with NULL, SHA256, RSA signatures, and
# key inbuilt to the header. This script internally invoke header generation
# application with corresponding parameters.
#
# Example Usage:
# To generate firmware image with NULL hash
# 	mk_c2kimage.sh null firmware.bin
#
# To generate firmware image with SHA256
#	mk_c2kimage.sh sha256 firmware.bin
#
# To generate firmware image with RSA signature and key inbuilt to the header
#	mk_c2kimage.sh rsa private.pem insertkey firmware.bin
#
# To generate firmware image with RSA signature and no key inserted in the header
#	mk_c2kimage.sh rsa private.pem nokey firmware.bin
#
# OpenSSL command to generate new private key
#	openssl genrsa -out private.pem 2048
#
# Note: RSA key size is determined from the given private key.
################################################################################

C2KIMAGE_GEN=$(pwd)/$(dirname $0)/c2kimage_gen

usage()
{
	echo "Usage..."
	echo "$0 <mode> <options> <fw_file>"
	echo " <mode>	    - legacy or nonlegacy"
	echo " <fw_file>    - Firmware image"
	echo "If legacy mode then the options are"
	echo "<base_offset>  - The value of base offset"
	echo "<start_offset> - The value of start offset"
	echo "If non-legacy mode then the options are"
	echo " <null>       - Place NULL hash in the header"
	echo " <sha256>     - Place SHA256 hash in the header"
	echo " <rsa> <private_key> <insertkey|nokey>"
	echo "              - Place RSA signature in the header using private_key"
	echo "                insertkey|nokey to inbuilt or exclude the key from header"
	echo " <gen_key> <private_key> <public_key>"
	echo "		    - Generate the public key and sha256 hash of this key"
	echo "              - Generate the public key with this name"
}

clean_exit()
{
	if [ -e "$public_modulus" ]
	then
		rm $public_modulus
	fi
	if [ -e "$sig_file" ]
	then
		rm $sig_file
	fi
	if [ -e "$hash_file" ]
	then
		rm $hash_file
	fi
	exit $1
}

check_image()
{
	if [ "$1" = "" ]; then
		usage
		clean_exit 1
	fi

	if [ ! -e $1 ]; then
		echo "Firmware file not found "$1
		clean_exit 1
	fi
}


case "$1" in

legacy)
	echo "Legacy mode..."
	base_off=$2
	start_off=$3
	image=$4

	echo "c2kimage_gen $1 $base_off $start_off $image"
	${C2KIMAGE_GEN} $1 $base_off $start_off $image
	;;

nonlegacy)
	echo "Non-Legacy mode..."
	case "$2" in

	null)
        	echo "Generating C2K image with NULL hash"
	        timestamp=`date +%s` || exit 1
        	image=$3

	        check_image $image

        	#Call header generation utility
		${C2KIMAGE_GEN} $1 $timestamp $image $2
	        echo "c2kimage_gen $1 $timestamp $image $2"
	        ;;

	sha256)
        	echo "Generating C2K image with SHA256.."
	        hash_file=`mktemp` || exit 1
        	timestamp=`date +%s` || exit 1
	        image=$3

        	check_image $image

	        #Calculate SHA256 hash on fw file.
	        openssl dgst -binary -sha256 < $image > $hash_file
        	if [ $? != 0 ]; then
	                echo "Error while calculating hash on $image"
        	        clean_exit 1
	        fi

        	#Call header generation utility
	        ${C2KIMAGE_GEN} $1 $timestamp $image $2 $hash_file
        	echo "c2kimage_gen $1 $timestamp $image $2 $hash_file"
	        ;;

	rsa*)
        	echo "Generating C2K image with RSA signature .."
	        public_modulus=`mktemp` || clean_exit 1
	        sig_file=`mktemp` || clean_exit 1
	        hash_file=`mktemp` || clean_exit 1
	        private_key=$3
	        keymode=$4
	        timestamp=`date +%s` || exit 1
	        image=$5

        	check_image $image

	        if [ ! -e $private_key ]; then
	                echo "Private key file not found '$private_key'"
        	        clean_exit 1
	        fi

        	#Calculate SHA256 hash on fw file.
	        openssl dgst -binary -sha256 < $image > $hash_file
	        #RSA Sign the hash using private key
	        openssl rsautl -sign -inkey $private_key -keyform PEM -pkcs -in $hash_file -out $sig_file

	        #extract the public modulus(N) from the private key.
	        openssl rsa -in $private_key -modulus | awk -F"=" '{if ($1 == "Modulus"){print $2}}' > $public_modulus

	        ${C2KIMAGE_GEN} $1 $timestamp $image $2 $sig_file $public_modulus $keymode
	        echo "c2kimage_gen $1 $timestamp $image $2 $sig_file $public_modulus $keymode"
	        ;;

	gen_key)
	        echo "Generating public key"
	        timestamp=`date +%s` || exit 1
	        public_modulus=`mktemp` || clean_exit 1
	        privatekey=$3
	        publickey=$4
	        image=$5
	        hashfile="publichash"

	        check_image $image

	        if [ ! -e $privatekey ]; then
	                echo "Private key file not found '$privatekey'"
	                clean_exit 1
	        fi

	        #extract the public modulus(N) from the private key.
	        openssl rsa -in $privatekey -modulus | awk -F"=" '{if ($1 == "Modulus"){print $2}}' > $public_modulus

	        #Call header generation utility
	        ${C2KIMAGE_GEN} $1 $timestamp $image $2 $publickey $public_modulus
	        echo "c2kimage_gen $1 $timestamp $image $2 $publickey $public_modulus"
	        #Calculate SHA256 hash on public key file.
	        openssl dgst -binary -sha256 < $publickey > $hashfile
	        echo "Generated hash on public key"
        	;;


	*)
	        usage
	        exit 1
	        ;;

	esac
	;;

*)
	usage
	exit 1
	;;

esac
clean_exit 0

