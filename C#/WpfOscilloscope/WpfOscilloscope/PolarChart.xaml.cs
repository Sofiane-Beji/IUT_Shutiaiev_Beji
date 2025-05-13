using SciChart.Charting.Model.DataSeries;
using SciChart.Charting.Visuals;
using SciChart.Charting.Visuals.Axes.LabelProviders;
using SciChart.Charting.Visuals.Axes;
using SciChart.Charting.Visuals.RenderableSeries;
using SciChart.Data.Model;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;


namespace PolarChartControl
{
    /// <summary>
    /// Logique d'interaction pour UserControl1.xaml
    /// </summary>
    public partial class PolarChart : UserControl
    {
        SciChartSurface sciChartSurface;
        public bool isDisplayActivated = false;

        DispatcherTimer displayTimer;
        FastLineRenderableSeries lineSeries;
        XyDataSeries<double, double> dataSeries;

        public PolarChart()
        {
            InitializeComponent();
            displayTimer = new DispatcherTimer(priority: DispatcherPriority.Background);
            displayTimer.Interval = new System.TimeSpan(0, 0, 0, 0, 100);
            displayTimer.Tick += DisplayTimer_Tick;
            displayTimer.Start();

            sciChartSurface = new SciChartSurface();
            sciChartSurface.XAxis = new PolarXAxis();
            sciChartSurface.YAxis = new PolarYAxis();

            var lineSeries = new FastLineRenderableSeries()
            {
                Stroke = Colors.White,
                AntiAliasing = true,
                StrokeThickness = 1
            };

            sciChartSurface.RenderableSeries.Add(lineSeries);

            dataSeries = new XyDataSeries<double, double>();
            
        }
        public void appendData(double xData, double yData) 
        {lineSeries.DataSeries = dataSeries;
            dataSeries.Append(xData, yData);
            
        }
        private void DisplayTimer_Tick(object sender, System.EventArgs e)
        {
            if (isDisplayActivated)
            {
                
                
            }
        }

        
    }
}
