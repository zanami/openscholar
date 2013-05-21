Install
=========
Download the git wrapper class from here: https://github.com/cpliakas/git-wrapper
into profiles/libraries/git.
In the folder run the next two commands:
curl -s https://getcomposer.org/installer | php
php composer.phar install

If the curl is not available run
php -r "eval('?>'.file_get_contents('https://getcomposer.org/installer'));"
php composer.phar install
