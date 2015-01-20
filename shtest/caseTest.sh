
case "$1" in
"start")
    ps -ef | grep "sleep" | grep -v grep | wc -l > /dev/null 2>&1
    if (($?==0));then
        echo "sleep start!"
        sleep 1000
    fi
    ;;
"stop")
    PID=$(ps -ef | grep "sleep" | grep -v grep | grep `whoami` | awk '{print $2}')
    if (($?==0));then
        kill -9 $PID 
        echo "sleep stop!"
    else
        echo "sleep not found!"
    fi
    ;;
*)
    echo "Usage:" $0 "{start:stop}"
    ;;
esac

