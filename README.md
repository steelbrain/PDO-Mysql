PDO-Mysql
===========
PDO-Mysql is a PDO wrapper for php, it's guranteed to make your life easy. It saves you from inventing the wheel again.
## Examples:
### Initialization
```php
<?php
$config = [
    'host'      => 'localhost',
    'user'      => 'steel',
    'password'  => 'brain',
    'db'        => 'records'
];
Database::init($config);
```
### Simple Queries
```php
$user = Database::fetch("Select username from `my_db`.`users` where id = :id LIMIT 1",[':id' => 3]);

$posts = Database::fetchAll("Select post_description,post_meta from `my_db`.`posts` post_author = :author",[':author' => 'steelbrain']);

DataBase::query("Update users set last_active = :time where id = :id LIMIT 1",[':id' => 8,':time' => time()]);
```
### Insert Example
```php
/*
    The Insert syntax is my favorite part
    DataBase::insert("TABLE",[
        'key1' => 'value1',
        'key2' => 'value2'
    ]);
*/
DataBase::insert('mytable',[
    'viewer_ip' => $_SERVER['REMOTE_ADDR'],
    'viewer_page' => $_SERVER['REQUEST_URI'],
    'time_viewed' => time()
]);
```
### Exists Example
```php
// in Exists you dont need to write the `Select [123123] from ` part
if(!Database::exists("my_db.table where id= :id",[':id' => 9]){
    DataBase::insert('my_db.my_table',[
        'id' => 5,
        'username' => 'john.doe',
        'name' => 'Rocky The Man',
        'location' => 'Middle of Nowhere',
        'last_seen' => time()
    ]);
}
```
### Using Select Results
// Note: Variables in double quotes are [parsed by php][1]
```php
$users = Database::fetchAll("Select username,user_id,display_name from users");
foreach($users as $user){
    echo "Username: $user[username]\n";
    echo "ID: $user[id]\n";
    echo "Name: $user[display_name]\n";
    echo "---------------------------";
}
```
[1]:http://php.net/manual/en/language.types.string.php