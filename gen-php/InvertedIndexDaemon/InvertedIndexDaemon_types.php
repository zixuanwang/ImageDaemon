<?php
/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
include_once $GLOBALS['THRIFT_ROOT'].'/Thrift.php';


class Posting {
  static $_TSPEC;

  public $imageId = null;
  public $score = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'imageId',
          'type' => TType::I64,
          ),
        2 => array(
          'var' => 'score',
          'type' => TType::DOUBLE,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['imageId'])) {
        $this->imageId = $vals['imageId'];
      }
      if (isset($vals['score'])) {
        $this->score = $vals['score'];
      }
    }
  }

  public function getName() {
    return 'Posting';
  }

  public function read($input)
  {
    $xfer = 0;
    $fname = null;
    $ftype = 0;
    $fid = 0;
    $xfer += $input->readStructBegin($fname);
    while (true)
    {
      $xfer += $input->readFieldBegin($fname, $ftype, $fid);
      if ($ftype == TType::STOP) {
        break;
      }
      switch ($fid)
      {
        case 1:
          if ($ftype == TType::I64) {
            $xfer += $input->readI64($this->imageId);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 2:
          if ($ftype == TType::DOUBLE) {
            $xfer += $input->readDouble($this->score);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        default:
          $xfer += $input->skip($ftype);
          break;
      }
      $xfer += $input->readFieldEnd();
    }
    $xfer += $input->readStructEnd();
    return $xfer;
  }

  public function write($output) {
    $xfer = 0;
    $xfer += $output->writeStructBegin('Posting');
    if ($this->imageId !== null) {
      $xfer += $output->writeFieldBegin('imageId', TType::I64, 1);
      $xfer += $output->writeI64($this->imageId);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->score !== null) {
      $xfer += $output->writeFieldBegin('score', TType::DOUBLE, 2);
      $xfer += $output->writeDouble($this->score);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

?>