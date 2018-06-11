if ps ax | grep -v grep | grep robot > \dev\null
then
ps -e -f | grep robot | grep -v grep | awk '{print $2}' | xargs kill
else
echo "That process does not exist."
fi
