.PHONY: clean all
sync_local2remote:
	@rsync -avzr -e "ssh " ./ 192.168.1.112:/home/william/code/lab6/

sync_remote2local:
	@rsync -avzr -e "ssh " 192.168.1.112:/home/william/code/lab6 ./
