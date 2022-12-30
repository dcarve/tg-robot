import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as ticker
import json
import configs
import calcs

class AnimationPlot:

    def animate(self, i, rpm_list, rpm2_list, millis_list, ser):
        decoded_bytes = ser.readline().decode("utf-8").strip('\r\n')
        decoded_bytes = '"'+decoded_bytes.replace(':','":').replace(',',',"')
        
        try:
            dict_data = json.loads('{' + decoded_bytes + '}')
            rpm_list.append(dict_data['rpm'])
            rpm2_list.append(dict_data['rpm2'])
            
            seconds = calcs.millis_to_seconds(dict_data['millis'])
            
            millis_list.append(seconds)

        except:
            pass

        rpm_list = rpm_list[-configs.CHUNK_DATA:]       
        rpm2_list = rpm2_list[-configs.CHUNK_DATA:]       
        millis_list = millis_list[-configs.CHUNK_DATA:]
       
        ax1.clear()
        ax2.clear()
        ax3.clear()
        ax4.clear()

        fcycles_values = calcs.fcycles(millis_list)
        fft_values_1 = calcs.fft_values(rpm_list)
        fft_values_2 = calcs.fft_values(rpm2_list)

        rpm_1_list_low_pass = calcs.low_pass_filter_rpm_1(rpm_list)
        rpm_2_list_low_pass = calcs.low_pass_filter_rpm_2(rpm2_list)

        fft_low_pass_values_1 = calcs.fft_values(rpm_1_list_low_pass)
        fft_low_pass_values_2 = calcs.fft_values(rpm_2_list_low_pass)


        self.plot_format()
        ax1.plot(millis_list, rpm_list, color='#3684eb', label='rpm 1')
        ax1.plot(millis_list, rpm2_list, color='#f76205', label='rpm 2')
        
        ax2.plot(fcycles_values, fft_values_1, color='#3684eb', label='freq 1')
        ax2.plot(fcycles_values, fft_values_2, color='#f76205', label='freq 2')


        ax3.plot(millis_list, rpm_1_list_low_pass, color='#3684eb', label='rpm 1')
        ax3.plot(millis_list, rpm_2_list_low_pass, color='#f76205', label='rpm 2')
        
        ax4.plot(fcycles_values, fft_low_pass_values_1, color='#3684eb', label='freq 1')
        ax4.plot(fcycles_values, fft_low_pass_values_2, color='#f76205', label='freq 2')

        ax1.legend()
        ax2.legend()
        ax3.legend()
        ax4.legend()
        
        
    def plot_format(self):
        
        ax1.set_ylim([0, 200])
        ax1.set_title("rpm")
        ax1.set_xlabel("secconds")
        ax1.set_ylabel("rpm")
        
        ax2.set_xlim([0, 50])
        ax2.set_ylim([10**(-1), 10**5])
        ax2.set_yscale('symlog')
        ax2.set_title("freq rpm")
        ax2.set_xlabel("Hz")
        
        
        ax3.set_ylim([0, 200])
        ax3.set_title("rpm passa baixa")
        ax3.set_xlabel("secconds")
        ax3.set_ylabel("rpm")
        
        ax4.set_xlim([0, 50])
        ax4.set_ylim([10**(-1), 10**5])
        ax4.set_yscale('symlog')
        ax4.set_title("freq rpm passa baixa")
        ax4.set_xlabel("Hz")


rpm_list = []
rpm2_list = []
millis_list = []
                                                        
fig = plt.figure()
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)

realTimePlot = AnimationPlot()

ser = serial.Serial(configs.PORT, configs.BAUDRATE)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)


ani = animation.FuncAnimation(fig, realTimePlot.animate, frames=configs.CHUNK_DATA, fargs=(rpm_list, rpm2_list, millis_list, ser), interval=10) 

plt.show()