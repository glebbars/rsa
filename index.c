#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int mod_inverse(int a, int m);
bool is_prime(int n);
void get_prime_values(int *p, int *q);
void generate_keys(int p, int q, int *e, int *d, int *n);
void encrypt(char *message, int *encrypted, int message_length, int e, int n);
void decrypt(int *encrypted, char *decrypted, int message_length, int d, int n);

int main()
{
  // Відкрита експонента, найчастіше рекомендується брати 65537 = 2^16+2
  int e;
  // Закрита експенента та добуток простих чисел p та q
  int d, n;

  // Запит значень p та q
  int p, q;
  get_prime_values(&p, &q);

  // Генерація ключів
  generate_keys(p, q, &e, &d, &n);

  // Відкриття вхідного файлу у форматі читання
  FILE *input_file = fopen("input.txt", "r");

  // Відкриття / створення вихідного файлу з зашифрованим повідомленням у форматі запису
  FILE *encrypted_output_file = fopen("encrypyed.txt", "w");

  // Відкриття / створення вихідного файлу з розшифрованим повідомленням у форматі запису
  FILE *decryped_output_file = fopen("decrypyed.txt", "w");

  // Обробка помилок при відкритті файлів
  if (input_file == NULL || encrypted_output_file == NULL || decryped_output_file == NULL)
  {
    printf("Помилка: не получилось видкрити вхідний або вихіді файли.\n");
    exit(1);
  }

  // Читання повідомлення з вхідного фалйу
  char message[1000];
  fgets(message, 1000, input_file);
  // Довжина повідомлення з вхідного фалйу
  int message_length = strlen(message);

  // Шифруванння повідомлення
  int encrypted[message_length];
  encrypt(message, encrypted, message_length, e, n);

  // Запис зашифрованого повідомлення у вихідний файл
  for (int i = 0; i < message_length; i++)
  {
    fprintf(encrypted_output_file, "%d ", encrypted[i]);
  }

  // Розшифруванння повідомлення
  char decrypted[message_length];
  decrypt(encrypted, decrypted, message_length, d, n);

  // Запис розшифрованого повідомлення у вихідний файл
  fprintf(decryped_output_file, "%s", decrypted);

  // Закриття файлів
  fclose(input_file);
  fclose(encrypted_output_file);
  fclose(decryped_output_file);

  return 0;
}

// Обчислити модульної множини, оберненої до числа за розширеним алгоритмом Евкліда
int mod_inverse(int e, int phi)
{
  int m0 = phi, t, q;
  int x0 = 0, x1 = 1;

  if (phi == 1)
  {
    return 0;
  }

  while (e > 1)
  {
    q = e / phi;
    t = phi;
    phi = e % phi;
    e = t;
    t = x0;
    x0 = x1 - q * x0;
    x1 = t;
  }

  if (x1 < 0)
  {
    x1 += m0;
  }

  return x1;
}

// Перевірка, чи є число простим
bool is_prime(int n)
{
  if (n <= 1)
  {
    return false;
  }
  for (int i = 2; i * i <= n; i++)
  {
    if (n % i == 0)
    {
      return false;
    }
  }
  return true;
}

// Запит на значення простих числе p та q у користувача
void get_prime_values(int *p, int *q)
{
  do
  {
    printf("Вкажіть значення p: ");
    scanf("%d", p);
    if (!is_prime(*p))
      printf("%d не є простим числом! Спробуйте знову.\n", *p);
  } while (!is_prime(*p));

  do
  {
    printf("Вкажіть значення q: ");
    scanf("%d", q);
    if (!is_prime(*q))
      printf("%d не є простим числом! Спробуйте знову.\n", *q);
  } while (!is_prime(*q));
}

// Генерація публічного та приватного ключів
void generate_keys(int p, int q, int *e, int *d, int *n)
{
  int phi = (p - 1) * (q - 1);
  *n = p * q;
  *e = 65537;
  *d = mod_inverse(*e, phi);

  printf("Публічний ключ: (%d, %d)\n", *e, *n);
  printf("Приватний ключ: (%d, %d)\n", *d, *n);
}

// Шифрування повідомлення, використовуючи публічний ключ
void encrypt(char *message, int *encrypted, int message_length, int e, int n)
{
  for (int i = 0; i < message_length; i++)
  {
    // переклад літер у числа через таблицю ASCII
    int ascii = (int)message[i];
    int cipher = 1;
    for (int j = 0; j < e; j++)
    {
      cipher = (cipher * ascii) % n;
    }
    encrypted[i] = cipher;
  }
}

// Розшифрування повідомлення, використовуючи приватний ключ
void decrypt(int *encrypted, char *decrypted, int message_length, int d, int n)
{
  for (int i = 0; i < message_length; i++)
  {
    int cipher = encrypted[i];
    int plain = 1;
    for (int j = 0; j < d; j++)
    {
      plain = (plain * cipher) % n;
    }
    decrypted[i] = (char)plain;
  }
  decrypted[message_length] = '\0';
}
