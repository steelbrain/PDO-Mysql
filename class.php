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
     public static $link = false;
     public static $lastInserted=false;
     public static function init($config){
          self::$link = new PDO('mysql:host='.$config['host'].';dbname='.$config['db'],$config['user'],$config['pass'],[PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8']);
     }
     public static function query($q,array $args = null){
          if(!$args)$args = [];
          $q = self::$link->prepare($q);
          $q->execute($args);
          return $q;
     }
     public static function fetch($q, array $args = null){
          if(!$args)$args = [];
          return self::query($q,$args)->fetch(PDO::FETCH_ASSOC);
     }
     public static function fetchAll($q, array $args = null){
          if(!$args)$args=[];
          return self::query($q,$args)->fetchAll(PDO::FETCH_ASSOC);
     }
     public static function exists($q, array $args = null){
          return (bool) self::query("Select 1 from $q LIMIT 1",$args)->rowCount();
     }
     public static function insert($table,$args){
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