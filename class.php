<?php
/**
 * @Package PDO-Mysql
 * @License SEE THE LICENSE FILE
 * @author Steel Brain <aneesiqbalbhatti@gmail.com>
 */
class Database{
     /**
      * @var PDO
      */
     public static $link = null;
     public static $lastInserted = 0;
     public static $config = [];
     public static $default_config = [
          'host' => 'localhost',
          'user' => 'root',
          'db' => '',
          'pass' => ''
     ];
     public function __construct(array $config = []){// non static, means multiple instances at once
          self::$config = array_merge(self::$default_config,$config);
          self::__init();
     }
     public static function __preinit(){ // set default stuff
          if(defined('DB_HOST')){
               self::$default_config['host'] = DB_HOST;
          }
          if(defined('DB_USER')){
               self::$default_config['user'] = DB_USER;
          }
          if(defined('DB_NAME')){
               self::$default_config['db'] = DB_NAME;
          }
          if(defined('DB_PASS')){
               self::$default_config['pass'] = DB_PASS;
          }
     }
     public static function init(array $config = []){
          self::$config = array_merge(self::$default_config,$config);
          self::__init();
     }
     private static function __init(){
          self::$link = new PDO('mysql:host='.self::$config['host'].';dbname='.self::$config['db'],self::$config['user'],self::$config['pass'],[PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8']);
          self::$link->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);
     }
     public static function query($q,array $args = null){
          if(self::$link === null){
               self::init();
          }
          if(!$args)$args = [];
          $q = self::$link->prepare($q);
          $q->execute($args);
          return $q;
     }
     public static function ping(){
          try {
               self::query("Select 1",null);
          } catch(PDOException $e){
               self::__init();
          }
     }
     public static function fetch($q, array $args = null){
          if(!$args)$args = [];
          return self::query($q,$args)->fetch(PDO::FETCH_ASSOC);
     }
     public static function fetchCol($q, array $args = null){
          if(!$args)$args = [];
          $query = self::query($q,$args)->fetch(PDO::FETCH_NUM);
          return isset($query[0]) ? $query[0] : '';
     }
     public static function fetchAll($q, array $args = null){
          if(!$args)$args=[];
          return self::query($q,$args)->fetchAll(PDO::FETCH_ASSOC);
     }
     public static function exists($q, array $args = null){
          return (bool) self::query("Select 1 from $q LIMIT 1",$args)->rowCount();
     }
     public static function insert($table,$args){
          if(self::$link === null)self::init();
          $keys = $values = $params = [];
          foreach($args as $key => &$value){
               if(is_array($value))$value = json_encode($value);
               $params[":$key"] = $value;
               $keys[] = $key;
               $values[] = ":$key";
          }
          $keys = implode(',',$keys);
          $values = implode(',',$values);
          self::query("Insert into $table ($keys) VALUES ($values)",$params);
          self::$lastInserted = self::$link->lastInsertId();
          return self::$lastInserted;
     }
}
Database::__preinit();