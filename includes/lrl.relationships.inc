<?php
/**
 * @file
 * Callbacks to update relationships.
 */


/**
 * Callback to update relationships between Experiment and Protocol.
 *
 * Update relationships if an Experiment object is ingested successfully.
 */
function islandora_lrl_experiment_rels_update(array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  $object = islandora_ingest_form_get_object($form_state);
  if ($metadata_step_storage['created']['DWC'] && $object) {
    $protocol = explode('(', trim($metadata_step_storage['values']['dwc_fields_experiment']['ResourceRelationship']));
    $protocol_id = substr($protocol[1], 0, -1);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isGovernedBy', $protocol_id);
  }
}


/**
 * Callback to update relationships between Experiment and DataSession.
 *
 * Update relationships if an DataSession object is ingested successfully.
 */
function islandora_lrl_datasession_rels_update(array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  $object = islandora_ingest_form_get_object($form_state);
  if ($metadata_step_storage['created']['DWC'] && $object) {
    $exp = explode('(', trim($metadata_step_storage['values']['dwc_fields_datasession']['ResourceRelationship']));
    $exp_id = substr($exp[1], 0, -1);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSessionOf', $exp_id);

    // Multiple data files associated with the datasession.
    $data_files = $metadata_step_storage['values']['dwc_fields_datasession']['datafilespanel'];
    $len = count($data_files);
    if ($len >= 1) {
      foreach ($data_files as $i => $data_file) {
        $file = $data_file['resourceID'];
        if (preg_match('/\s/', $file)) {
          $file = preg_replace('/\s+/', '_', $file);
        }
        $mice[$i] = explode('(', trim($data_file['relresourceID']));
        $mice_id = substr($mice[$i][1], 0, -1);
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isDataMeasurementOf', $mice_id);
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasDataFile', $file);
      }
    }
  }
}


/**
 * Callback to update relationships between Mice.
 *
 * Update relationships if a Mouse object is ingested successfully.
 */
function islandora_lrl_mouse_rels_update(array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  $object = islandora_ingest_form_get_object($form_state);
  if ($metadata_step_storage['created']['DWC'] && $object) {
    if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid'])) {
      $mother = $metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid'];
      $m_pid = explode('(', trim($mother));
      $pid = substr($m_pid[1], 0, -1);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasMother', $pid);
    }
    if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid'])) {
      $father = $metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid'];
      $f_pid = explode('(', trim($father));
      $pid = substr($f_pid[1], 0, -1);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasFather', $pid);
    }
    if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'][0]['locationid1'])
    || !empty($metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'][1]['locationid1'])) {
      $cage_history = $metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'];
      foreach ($cage_history as $cage) {
        if (trim($cage['locality1']) == 'currentCage') {
          $cage_id = $cage['locationid1'];
        }
      }
      $cage_pid = explode('(', trim($cage_id));
      $pid = substr($cage_pid[1], 0, -1);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isCurrentResidentOf', $pid);
    }

    if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid'])) {
      $protocol = $metadata_step_storage['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid'];
      $p_pid = explode('(', trim($protocol));
      $pid = substr($p_pid[1], 0, -1);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isTestSubjectUnder', $pid);
    }


  }
}