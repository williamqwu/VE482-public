# code example for PS3
PS3='Select a number (1-3): '
select choice in 'Apple' 'Banana' 'Cabbage'
do
    echo -ne "Today's breakfast is "
    case $choice in
        Apple) echo apple ;;
        Banana) echo banana;;
        Cabbage) echo cabbage;;
        *) echo nothing;;
    esac
    break
done