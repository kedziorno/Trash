using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Threading;
using System.Management;
using System.Management.Instrumentation;

namespace ServerMonitor
{
    class Program
    {
        static void Main(string[] args)
        {
            int Index = 0, RetrieveCount = 2;

            List<CpuUsage> cu = new List<CpuUsage>();

            List<Query> _query = new List<Query>()
            {
                new Query {  Attributes = "IDProcess,PercentProcessorTime,Timestamp_Sys100NS", ClassName = "Win32_PerfRawData_PerfProc_Process", ServerAddress = "root\\CIMV2" }
            };

            for(Index = 0; Index < RetrieveCount; Index++)
            {
                foreach (Query iQuery in _query)
                {
                    ManagementScope scope = new ManagementScope(iQuery.ServerAddress);
                    scope.Connect();

                    ObjectQuery qo = new ObjectQuery(iQuery.GetSelectQuery());
                    ManagementObjectSearcher mos = new ManagementObjectSearcher(scope, qo);
                    ManagementObjectCollection moc = mos.Get();

                    foreach (ManagementObject mo in moc)
                    {
                        CpuUsage cuItem = new CpuUsage();

                        foreach (PropertyData data in mo.Properties)
                        {
                            //Console.WriteLine("{0}: {1}", data.Name, data.Value);
                            UInt32 ProcessId;

                            if (data.Name == "IDProcess")
                            {
                                ProcessId = UInt32.Parse(data.Value.ToString());
                                cuItem.ProcessId = ProcessId;
                                foreach (CpuUsage inList in cu)
                                {
                                    if (inList.ProcessId == cuItem.ProcessId)
                                    {
                                        cu.Add(cuItem);
                                    }
                                }
                            }

                            if (Index == 0)
                            {
                                if (data.Name == "PercentProcessorTime")
                                    cuItem.PercentProcessorTime_1 = UInt64.Parse(data.Value.ToString());
                                if (data.Name == "Timestamp_Sys100NS")
                                    cuItem.Timestamp_Sys100NS_1 = UInt64.Parse(data.Value.ToString());
                            }

                            if (Index == 1)
                            {
                                if (data.Name == "PercentProcessorTime")
                                    cuItem.PercentProcessorTime_2 = UInt64.Parse(data.Value.ToString());
                                if (data.Name == "Timestamp_Sys100NS")
                                    cuItem.Timestamp_Sys100NS_2 = UInt64.Parse(data.Value.ToString());
                            }

                           
                        }
                    }
                }
            }
           
            foreach(CpuUsage item in cu)
            {
                Console.WriteLine(item.dump());
            }

            /*
            while(true)
            {
                Console.SetCursorPosition(0, 0);
                Process ProcessItem;
                Process[] ProcessArray = Process.GetProcesses();
                Int32 ProcessArrayLen = ProcessArray.Length, IndexInTable;
                for (IndexInTable = 0; IndexInTable < ProcessArrayLen; IndexInTable++)
                {
                    ProcessItem = ProcessArray[IndexInTable];
                    int ProcessId = ProcessItem.Id;
                    String ProcessName = ProcessItem.ProcessName;

                    PerformanceCounter myAppCpu =
                    new PerformanceCounter(
                        "Process", "% Processor Time", ProcessName, true);

                    double pct = myAppCpu.NextValue();
                    Console.WriteLine("PID: {0}, % CPU: {1}", ProcessId, pct);
                }
                Thread.Sleep(1000);
            }
            */

            /*
            Console.WriteLine("Press the any key to stop...\n");
            while (!Console.KeyAvailable)
            {
                double pct = myAppCpu.NextValue();
                Console.WriteLine("OUTLOOK'S CPU % = " + pct);
               
            }
            */
            Console.Read();
        }
    }
}


