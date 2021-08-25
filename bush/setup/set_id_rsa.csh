
eval `ssh-agent -c`
ssh-add ~/.ssh/id_rsa

#set old_SHELL=`printenv SHELL`
#setenv SHELL csh
#eval `keychain --agents ssh --eval id_rsa`
#setenv SHELL "${old_SHELL}"




