int insert(int value, std::vector<int> &table) {

  int hashIndex = value % 1000;
  int hashCount = 0;

  if (table[hashIndex] == -1)
    {
      table[hashIndex] = value;
      return 0;
    }
  else
  {
      while (table[hashIndex] != -1)
      {
        hashIndex = (hashIndex+1)%1000;
        hashCount++;
        hashIndex %= table.size();
      }
      table[hashIndex] = value;
    }
  // Code to insert value into a hashed location in table
  // where table is a vector of length 1000.
  // Returns the number of collisions encountered when
  // trying to insert value into table.
  return hashCount;
}
