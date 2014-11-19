PDO-Mysql
===========
PDO-Mysql is a PDO wrapper for php, it's guranteed to make your life easy. It saves you from inventing the wheel again.
## Examples:

### Initialization

####Method 1
```php
<?php
$config = [
    'host'      => 'localhost',
    'user'      => 'steel',
    'pass'      => 'brain',
    'db'        => 'records'
];
Database::init($config);
// You can execute the SQL queries on the Database class
```

####Method 2

```php
<?php
$config = [
    'host'      => 'localhost',
    'user'      => 'steel',
    'pass'      => 'brain',
    'db'        => 'records'
];
$db_1 = new Database($config);
// All calls should be done upon $db_1
// For example:
$db_1::insert("mytable",[
    'col_a' => 'val_a',
    'col_b' => 'val_b'
]);
```

####Method 3
```php
// Initialize with default configuration
<?php
Database::init();
```

####Method 4
```php
<?php
// Initialize an instance with default values
$db = new Database();
```

####Method 5
```php
<?php
// Altering the default values
// They are all optional, and will be used if defined, as default
define('DB_HOST','192.168.0.1');
define('DB_USER','myuser');
define('DB_NAME','mydb');
define('DB_PASS','MyPa$$wOrd');
$db = new DataBase(); // Or
Database::init();
```

### Simple Queries
```php
// Fetching a single column
$username = Database::fetchCol("Select username from users where id = :id LIMIT 1",[
    ':id' => 3
]);

// Fetching multiple columns, on a Database instance
$info = $db::fetch("Select id, username, status from users where id = :id LIMIT 1",[
    ':id' => 4
]);

// Fetching multiple rows, on a DataBase instance
$users = $db::fetch("Select id, username, status from users LIMIT 5 ORDER by id DESC",[
     ':id' => 5
]);

// Executing a raw query
Database::query("SET NAMES `utf8mb4`");

// .. with parameters
DataBase::query("Update users set last_active = :time where id = :id LIMIT 1",[
     ':id' => 8,
     ':time' => time()
]);
```
### Insert Example
```php
/*
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
// in Exists you dont need to write the `Select XYZ from ` and limit part
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
### Using Results
```php
// Note: Variables in double quotes are [parsed by php][1]
$users = Database::fetchAll("Select username,user_id,display_name from users");
foreach($users as $user){
    echo "Username: $user[username]\n";
    echo "ID: $user[id]\n";
    echo "Name: $user[display_name]\n";
    echo "---------------------------";
}
```
[1]:http://php.net/manual/en/language.types.string.php