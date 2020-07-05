CC="`head -1 conf-cc`"
set $CC
cc_arg=$1
shift
echo "if [ -n \"\$CC\" ] ; then"
echo "  " echo \""\$CC" \$CFLAGS $* -c '${1+"$@"}'\"
echo "  "   exec "\$CC" \$CFLAGS $* -c '${1+"$@"}'
echo "else"
echo "  " echo \""$cc_arg" \$CFLAGS $* -c '${1+"$@"}'\"
echo "  "   exec "$cc_arg" \$CFLAGS $* -c '${1+"$@"}'
echo "fi"
