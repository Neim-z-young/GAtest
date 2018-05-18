clear

load .\data.txt
a=size(data);
plot(data(1:a(1),1),data(1:a(1),2));
title('适应度收敛曲线');
xlabel('迭代次数');
ylabel('适应度');
axis on;
grid on;