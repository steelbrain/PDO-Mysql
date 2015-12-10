<?hh

namespace steelbrain;
use Exception;
use PDO;

class MySQLQuery<T> {
  public array<T> $Results = [];
  public bool $executed = false;
  public string $queryType = '';
  public shape(
    'Table' => string,
    'Columns' => string,
    'Where' => string,
    'Params' => array<string, mixed>,
    'Update' => string,
    'Sort' => string,
    'Limit' => int,
    'Offset' => int
  ) $Params = shape(
    'Table' => '',
    'Columns' => '',
    'Where' => '',
    'Params' => [],
    'Update' => '',
    'Sort' => '',
    'Limit' => 1,
    'Offset' => 0
  );
  public function __construct(public MySQL $link) { }
  public function from(string $table): this {
    $this->Params['Table'] = $table;
    return $this;
  }
  public function select($columns): this {
    if (is_array($columns)) {
      $this->Params['Columns'] = implode(', ', $columns);
    } else {
      invariant(is_string($columns), 'Columns parameter is not a string');
      $this->Params['Columns'] = $columns;
    }

    return $this;
  }
  public function update($clause): void {
    if ($this->executed) {
      throw new Exception('Query has already been executed');
    }

    if (is_array($clause)) {
      $clauses = [];
      foreach($clause as $key => $value) {
        $this->Params['Params'][":update_$key"] = $value;
        $clauses[] = "$key = :update_$key";
      }
      $this->Params['Update'] = implode(', ', $clauses);
    } else {
      invariant(is_string($clause), 'Clause parameter is not a string');
      $this->Params['Update'] = $clause;
    }

    $this->execute('update');
  }
  public function where($clause): this {
    if (is_array($clause)) {
      $clauses = [];
      foreach($clause as $key => $value) {
        $this->Params['Params'][":where_$key"] = $value;
        $clauses[] = "$key = :where_$key";
      }
      $this->Params['Where'] = implode(' AND ', $clauses);
    } else {
      invariant(is_string($clause), 'Clause parameter is not a string');
      $this->Params['Where'] = $clause;
    }

    return $this;
  }
  public function orderBy($clause): this {
    if (is_array($clause)) {
      $clauses = [];
      foreach($clause as $key => $value) {
        if (is_int($key)) {
          $clauses[] = $value;
        } else {
          $clauses[] = "$key $value";
        }
      }
      $this->Params['Sort'] = implode(', ', $clauses);
    } else {
      invariant(is_string($clause), 'Clause parameter is not a string');
      $this->Params['Sort'] = $clause;
    }

    return $this;
  }
  public function limit(int $limit): this {
    $this->Params['Limit'] = $limit;
    return $this;
  }
  public function offset(int $offset): this {
    $this->Params['Offset'] = $offset;
    return $this;
  }
  public function exists(): bool {
    if (!$this->executed) {
      $this->Params['Limit'] = 1;
      $this->execute('exists');
    }
    return array_key_exists(0, $this->Results);
  }
  public function delete(): void {
    if ($this->executed) {
      throw new Exception('Query has already been executed');
    }
    $this->execute('delete');
  }
  public function get(): ?T {
    if (!$this->executed) {
      $this->Params['Limit'] = 1;
      $this->execute('select');
    } else if ($this->queryType !== 'select') {
      throw new Exception('Cannot get results from a non-select query');
    }
    return array_key_exists(0, $this->Results) ? $this->Results[0] : null;
  }
  public function getAll(): array<T> {
    if (!$this->executed) {
      $this->execute('select');
    } else if ($this->queryType !== 'select') {
      throw new Exception('Cannot get results from a non-select query');
    }
    return $this->Results;
  }

  private function execute(string $queryType): void {
    $this->queryType = $queryType;
    if ($this->Params['Where'] === '') {
      $where = '';
    } else $where = 'WHERE '.$this->Params['Where'];
    if ($this->Params['Sort'] === '') {
      $sort = '';
    } else $sort = 'ORDER BY '.$this->Params['Sort'];

    if ($queryType === 'update') {
      $prefix = 'UPDATE '.$this->Params['Table'].' SET '.$this->Params['Update'];
    } else if ($queryType === 'exists') {
      $prefix = 'SELECT 1 from '.$this->Params['Table'];
    } else if ($queryType === 'delete') {
      $prefix = 'DELETE from '.$this->Params['Table'];
    } else {
      // select
      $prefix = "Select ".$this->Params['Columns'].' from '.$this->Params['Table'];
    }

    $statement = "$prefix $where $sort LIMIT ". $this->Params['Limit'].' OFFSET '.$this->Params['Offset'];
    $query = $this->link->query($statement, $this->Params['Params']);
    if ($queryType !== 'delete' && $queryType !== 'update') {
      $this->Results = $query->fetchAll(PDO::FETCH_ASSOC);
    }
    $this->executed = true;
  }
}
