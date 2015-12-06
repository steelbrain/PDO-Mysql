<?hh

namespace steelbrain;
use Exception;

class MySQLQuery<T> {
  public array<T> $Results = [];
  public bool $executed = false;
  public string $queryType = '';
  public shape(
    'Table' => string,
    'Columns' => string,
    'Where' => string,
    'Params' => array<string, mixed>,
    'Limit' => int
  ) $Params = shape(
    'Table' => '',
    'Columns' => '',
    'Where' => '',
    'Params' => [],
    'Limit' => 1
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
  public function limit(int $limit): this {
    $this->Params['Limit'] = $limit;
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
      $Where = '';
    } else $Where = "WHERE $this->Params[Where]";

    if ($queryType === 'exists') {
      $Prefix = 'SELECT 1';
    } else if ($queryType === 'delete') {
      $Prefix = 'DELETE';
    } else {
      // select
      $Prefix = "Select $this->Params[Columns]";
    }

    $statement = "$Prefix from $this->Params[Table] $Where LIMIT $this->Params[Limit]";
    $this->Results = $this->link->query($statement, $this->Params['Params'])->fetchAll(PDO::FETCH_ASSOC);
    $this->executed = true;
  }
}
