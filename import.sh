#!/bin/bash

# Usage example:
# bash import.sh /tmp/backup-file.sql http://localhost/openscholar/www

cd www
drush sql-drop -y
echo "Importing SQL file."
`drush sql-connect` < $1

# Skip update functions using a prefix
sed -i 's/os_files_update/_os_files_update/g' ../openscholar/modules/os/modules/os_files/*.install
sed -i 's/vsite_register_update/_vsite_register_update/g' ../openscholar/modules/vsite/modules/vsite_register/*.install
drush updb -y
# Return functions to original state
sed -i 's/_os_files_update/os_files_update/g' ../openscholar/modules/os/modules/os_files/*.install
sed -i 's/_vsite_register_update/vsite_register_update/g' ../openscholar/modules/vsite/modules/vsite_register/*.install

drush fra -y
drush cc all
drush vrd -y --uri=$2
drush dl acquia_connector -y --uri=$2
drush en acquia_search -y
drush en dblog -y
drush en devel -y
drush en os_search_solr -y

# Remove existing Solr index.
drush php-eval "apachesolr_environment_delete('acquia_search_server_1');"

# Add a local Solr index.
drush php-eval "apachesolr_environment_save(array('env_id' => 'default', 'name' => 'default', 'url' => 'http://localhost:8983/solr', 'service_class' => ''));"


# Index Solr.
# @todo: Allow switch to index Solr.
# drush solr-mark-all; drush solr-index

# Set teh file system
drush vset file_public_path "sites/default/files"
drush vset file_private_path ""
drush vset file_temporary_path "/tmp"

# Open site as admin.
drush uli --uri=$2



