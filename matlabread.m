clear

load .\data.txt
a=size(data);
plot(data(1:a(1),1),data(1:a(1),2));
title('��Ӧ����������');
xlabel('��������');
ylabel('��Ӧ��');
axis on;
grid on;