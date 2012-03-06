<?php
/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
include_once $GLOBALS['THRIFT_ROOT'].'/Thrift.php';


class Bin {
  static $_TSPEC;

  public $visualwordID = null;
  public $frequency = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'visualwordID',
          'type' => TType::I64,
          ),
        2 => array(
          'var' => 'frequency',
          'type' => TType::DOUBLE,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['visualwordID'])) {
        $this->visualwordID = $vals['visualwordID'];
      }
      if (isset($vals['frequency'])) {
        $this->frequency = $vals['frequency'];
      }
    }
  }

  public function getName() {
    return 'Bin';
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
            $xfer += $input->readI64($this->visualwordID);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 2:
          if ($ftype == TType::DOUBLE) {
            $xfer += $input->readDouble($this->frequency);
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
    $xfer += $output->writeStructBegin('Bin');
    if ($this->visualwordID !== null) {
      $xfer += $output->writeFieldBegin('visualwordID', TType::I64, 1);
      $xfer += $output->writeI64($this->visualwordID);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->frequency !== null) {
      $xfer += $output->writeFieldBegin('frequency', TType::DOUBLE, 2);
      $xfer += $output->writeDouble($this->frequency);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

?>
