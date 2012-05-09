<?php

/**
 * Theme api stuff. Get/set/list themes etc.
 */

/**
 * Returns HTML for the Powered by Openscholar text.
 *
 * @ingroup themeable
 */
function theme_openscholar_powered_by() {
  return '<span>' . t('Powered by <a href="@poweredby">OpenScholar</a>', array('@poweredby' => 'http://openscholar.harvard.edu')) . '</span>';
}