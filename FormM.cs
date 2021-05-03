using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GenAlg
{
    public partial class FormM : Form
    {
        public FormM()
        {
            InitializeComponent();
        }

        List<double> population = new List<double>();
        List<double> Func = new List<double>();
        int pop_size;
        Random rand = new Random();

        double Function(double x) //вычисление функции - второй производной
        {
            return 6.75 * Math.Pow(0.6667 * x - 1, 2.0);
        }

        double Mutaciya(double x0, double x1)  // мутация: генерация случайной величины
        {
            const int N = 100000000;            
            return Math.Abs((double)(rand.Next(N) * (x1 - x0) / N)) + x0;
        }

        double Inversiya(double x, double ee)  // инверсия: поиск в окрестностях точки
        {
            int znak = 0;
            znak++;
            znak %= 2;
            if (znak == 0) 
                return x - ee;
            else 
                return x + ee;
        }

        void Crossover(List<double> x, double eps, double x0, double x1)  
            // кроссовер: вычисляем среднее арифметическое между двумя
        {
            int k = pop_size-1;
            for (int i = 0; i < pop_size/5; i++)
                for (int j = i + 1; j < pop_size/5; j++)
                {
                    if (k < 0) break;
                    x[k] = (x[i] + x[j]) / 2;
                    k--;
                }
            for (int i = 0; i < pop_size/5; i++)
            {
                if (k < 0) break;
                x[k] = Inversiya(x[i], eps); 
                k--;
                if (k < 0) break;
                x[k] = Inversiya(x[i], eps); 
                k--;
            }
            for (int i = pop_size/5; i < k; i++)
                x[i] = Mutaciya(x0, x1);
        }

        void Sort(List<double> x, List<double> y)  // сортировка
        {
            for (int i = 0; i < pop_size; i++)
                for (int j = i + 1; j < pop_size; j++)
                    if (Math.Abs(y[j]) < Math.Abs(y[i]))
                    {
                        double help = y[i];
                        y[i] = y[j];
                        y[j] = help;
                        help = x[i];
                        x[i] = x[j];
                        x[j] = help;
                    }
        }

        double GeneticAlg(double x0, double x1, double eps)  // поиск решения с использованием ГА
        {
            int iter = 0;
            richTextBox1.Text += "Формирование начальной популяции\n";
            string s = "[";
            for (int i = 0; i < pop_size; i++)   // Формирование начальной популяции
            {
                population.Add(Mutaciya(x0, x1));
                Func.Add(Function(population[i]));
                s += this.population[i] + ", ";                
            }
            s = s.Substring(0, s.Length - 2);
            s += "]\n";
            richTextBox1.Text += s;            

            Sort(population, Func);

            do
            {
                iter++;
                richTextBox1.Text += "\n=== " + iter.ToString() + " поколение ===\n";
                s = "[";
                Crossover(population, eps, x0, x1); //выполнение процедуры кроссовера
                for (int i = 0; i < pop_size; i++)
                {
                    Func[i] = Function(population[i]);
                    s += this.population[i] + ", ";
                }
                s = s.Substring(0, s.Length - 2);
                s += "]\n";
                richTextBox1.Text += s;

                Sort(population, Func);
            }
            while (Math.Abs(Func[0]) > eps && iter < 20000);
            richTextBox1.Text += "\n=== " + iter.ToString() + " поколение ===\n";
            label3.Text = "Количество поколений:" + iter.ToString();
            return population[0];
        }

        private void button1_Click(object sender, EventArgs e)
        {
            pop_size = int.Parse(textBox2.Text); //размерность популяции              
            DateTime dt = DateTime.Now;
            //Выполняем генерирование особей            
            label4.Text = "Наилучшая хромосома:" + GeneticAlg(float.Parse(textBox3.Text), 
                float.Parse(textBox4.Text), float.Parse(textBox1.Text)).ToString();
            richTextBox1.Text += "Время работы программы:" + (DateTime.Now - dt).ToString();
            label5.Text = "Время работы программы:" + (DateTime.Now - dt).ToString();
        }

        private void FormM_Load(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
    }
}
