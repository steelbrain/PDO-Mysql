<?hh

namespace steelbrain;

class MySQLQuery<T> {
  public array<T> $Results = [];
  public bool $executed = false;
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
  public function get(): ?T {
    if (!$this->executed) {
      $this->execute();
    }
    return array_key_exists(0, $this->Results) ? $this->Results[0] : null;
  }
  public function getAll(): array<T> {
    if (!$this->executed) {
      $this->execute();
    }
    return $this->Results;
  }

  private function execute(): void {
    if ($this->Params['Where'] === '') {
      $Where = '';
    } else $Where = "WHERE $this->Params[Where]";
    $statement = "Select $this->Params[Columns] from $this->Params[Table] $Where LIMIT $this->Params[Limit]";
    $this->Results = $this->link->fetchAll($statement, $this->Params['Params']);
    $this->executed = true;
  }
}
