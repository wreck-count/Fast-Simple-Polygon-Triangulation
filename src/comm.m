clf;
n = 50;
x = 100*rand(1,n);
y = 100*rand(1,n);
%Find the order of the points
[~,porder] = sort(atan2(mean(y)-y,mean(x)-x));
xsort = x(porder);
ysort = y(porder);
%Plot polygon
twod = [xsort(:), ysort(:)];
twod = flip(twod);
hold on;
plot([xsort,xsort(1)],[ysort,ysort(1)],'k-');
writematrix(twod,'pgn.dat','delimiter',' ');
system('dcel_triangulate.exe');
s = dir('mne.dat');
if s.bytes == 0
    % empty file
else
    load mne.dat;
    xx = [mne(:,1) mne(:,3)];
    yy = [mne(:,2) mne(:,4)];
    plot(xx',yy','b-');
end
s = dir('tgn.dat');
if s.bytes == 0
    % empty file
else
    load tgn.dat;
    xxx = [tgn(:,1) tgn(:,3)];
    yyy = [tgn(:,2) tgn(:,4)];
    plot(xxx',yyy','r--');
end