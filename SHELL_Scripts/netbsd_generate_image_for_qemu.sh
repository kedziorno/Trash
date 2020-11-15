#!/bin/sh

if [ `id -u` -ne 0 ];
then
    echo "UID != 0";
    exit;
elif [ $# -lt 2 ];
then
    echo "$0: file.iso file.img [image size in MB (default: 10GB)] [image block size (default: 1MB)]";
    exit;
else
######## Init
    _pwd=`pwd`;

    src=src;
    dst=dst;
    vndsrc=vnd0;
    vnddst=vnd1;
   
    arch=i386;
    from=$src/$arch/binary/sets;
    to=$dst;

    iso_name=$1;
    image_name=$2;
    image_size=${3:-`expr 10 * 1024`};
    image_size=`echo $image_size | sed 's/[a-zA-Z]//g'`;
    image_block=${4:-1}
    image_block="`echo $image_block | sed 's/[a-zA-Z]//g'`m";
    partition_size=`df -m $0 | grep "^/.*" | awk -F" " '{print $4;}'`
    partition_name=`df -m $0 | grep "^/.*" | awk -F" " '{print $1;}'`
    dd_exec="dd if=/dev/zero of=$image_name bs=$image_block count=$image_size"
######## Q on image
    delete_image=0;
    if [ $image_size -lt 700 ];
    then
        echo "Error: $image_name < 700 MB";
        exit 1;
    else
        echo -n "Continue with $image_size MB ? (y/n) ";
        read cont;
        case $cont in
            y|Y) ;;
            n|N) exit 1;;
        esac
    fi
######## Delete old resources if exist
    echo "Prepend ... ";
    umount $src 2>&-;
    umount $dst 2>&-;
    rm -rf $src 2>&-;
    rm -rf $dst 2>&-;
    vnconfig -u $vndsrc 2>&-;
    vnconfig -u $vnddst 2>&-;
    rm -rf tmp 2>&-;
######## Create image file
    if [ $image_size -ge $partition_size ];
    then
        echo "No space left available on $partition_name ($partition_size MB vs $image_size MB)";
        exit 1;
    else
        echo "Create image $image_name ($image_size MB, block $image_block) ... ";
        if [ -f $image_name ];
        then
            exist_file_size=`ls -al $_pwd | grep $image_name | awk -F" " '{print $5;}'`;
            exist_file_size=`expr $exist_file_size / 1024 / 1024`
            echo -n "File exists (size $exist_file_size MB), Replace or Delete (and exit) ? (R/D/[Enter])";
            read replace;
            case $replace in
                r|R)
                    rm -f $image_name;
                    echo "Creating new file ... ";
                    $dd_exec 2>&-;
                    break;
                    ;;
                d|D)
                    rm -f $image_name;
                    echo "Deleting file.";
                    exit 1;
                    ;;
                *)
                    echo "File $image_name still exist.";
                    ;;
            esac
        else   
            $dd_exec 2>&-;
        fi
        if [ -f $image_name ];
        then
        else
            echo "error.";
            exit 1;
        fi
######## vnconfig
        echo -n "vnconfig for ISO file ... ";
        vnconfig $vndsrc $iso_name 2>&-;
        if [ $? -gt 0 ];
        then
            echo "error.";
            if [ $delete_image -eq 1 ];
            then
                rm -f $image_name;
            fi
            exit 1;
        fi
        echo "";

        echo -n "vnconfig for IMAGE file ... ";
        vnconfig $vnddst $image_name 2>&-;
        if [ $? -gt 0 ];
        then
            echo "error.";
            if [ $delete_image -eq 1 ];
            then
                rm -f $image_name;
            fi
            vnconfig -u $vndsrc;
            exit 1;
        fi
        echo "";
######## fs on image
        echo -n "Creating ffs on IMAGE file ? (Y/[N]) ";
        read cffs;
        case $cffs in
            y|Y)     tmpdl="dl_$vnddst"; mkdir tmp;
                part0d=`expr ( $image_size * 1024 * 1024 ) / 512`;
                part0a=`expr $part0d - 63`;
                fdisk -f -i $vnddst;
                fdisk -f -a -v -u -0 -s 169/63/$part0a $vnddst;
                #echo "bytes/sector: 512" >> tmpdl;
                disklabel $vnddst > tmp/a;
                cat tmp/a | grep -v "^.a:.*4.2BSD.*$" > tmp/b;
                cat tmp/b | grep -v "^.d:.*unused.*$" > $tmpdl;
                echo "a:    $part0a    63    4.2BSD    0    0    0" >> $tmpdl;
                echo "d:    $part0d    0    unused    0    0" >> $tmpdl;
                disklabel -R $vnddst $tmpdl;
                newfs -O 2 -o time /dev/r${vnddst}a 2>&-;
                rm -rf tmp/;
                #exit 1;
                break;
                ;;
            n|N)    break;
                ;;
        esac
        echo "";
######## Dirs
        echo -n "Making tmp dirs ... ";
        echo -n "$src ";
        mkdir $src 2>&-;
        if [ $? -gt 0 ];
        then
            echo "exist, ";
        fi

        echo -n "$dst ";
        mkdir $dst 2>&-;
        if [ $? -gt 0 ];
        then
            echo "exist";
        fi
        echo "";
######## Mount
        echo -n "Mounting $src ... ";
        mount /dev/${vndsrc}a $src 2>&-;
        if [ $? -gt 0 ];
        then
            echo "$vndsrc error.";
            vnconfig -u $vndsrc;
            vnconfig -u $vnddst;
            exit 1;
        fi
        echo "";

        echo -n "Mounting $dst ... ";
        mount /dev/${vnddst}a $dst 2>&-;
        if [ $? -gt 0 ];
        then
            echo "$vnddst error.";
            vnconfig -u $vndsrc;
            vnconfig -u $vnddst;
            exit 1;
        fi
        echo "";
######## Installing, configuring ,,,
        echo "Installing base system ... ";
        echo "kern-GENERIC.tgz ... ";
        tar -zxf $from/kern-GENERIC.tgz -C $dst 2>&-;
        echo -n "Files: ";
        j=0;
        for i in `ls $from/[a-jl-z]*.tgz`;
        do
            tar -zxf $i -C $dst;
            j=`expr $j + 1`;
            echo -n "$j ";
        done;
        echo -n "."; echo;
        cp $dst/usr/mdec/boot $dst/boot;
        $dst/usr/sbin/installboot -v /dev/r${vnddst}a $dst/usr/mdec/bootxx_ffsv2;   
######## End
    fi

#Cleaning
    cd $_pwd;
    umount $src;
    umount $dst;
    rm -rf $src 2>&-;
    rm -rf $dst 2>&-;
    vnconfig -u $vndsrc;
    vnconfig -u $vnddst;
    rm -rf tmp;
    rm -rf $tmpdl;
fi
