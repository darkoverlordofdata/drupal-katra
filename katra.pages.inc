<?php
// $Id$

/**
 * @file
 * Page callback file for the single game module.
 */

/**
 * Menu callback; displays a Drupal page containing recent game entries.
 */
function katra_page_last() {
  $build = array();

  $query = db_select('node', 'n')->extend('PagerDefault');
  $nids = $query
    ->fields('n', array('nid', 'sticky', 'created'))
    ->condition('type', KATRA_NODE_TYPE)
    ->condition('status', 1)
    ->orderBy('sticky', 'DESC')
    ->orderBy('created', 'DESC')
    ->limit(variable_get('default_nodes_main', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    drupal_set_message(t('No game entries have been created.'));
  }
  drupal_add_feed('game/feed', t('RSS - game'));

  return $build;
}

/**
 * Menu callback; displays an RSS feed containing recent game entries.
 */
function katra_feed_last() {
  $nids = db_select('node', 'n')
    ->fields('n', array('nid', 'created'))
    ->condition('type', 'game')
    ->condition('status', 1)
    ->orderBy('created', 'DESC')
    ->range(0, variable_get('feed_default_items', 10))
    ->addTag('node_access')
    ->execute()
    ->fetchCol();

  $channel['title'] = t('!site_name game', array('!site_name' => variable_get('site_name', 'Drupal')));
  $channel['link'] = url('game', array('absolute' => TRUE));

  node_feed($nids, $channel);
}
