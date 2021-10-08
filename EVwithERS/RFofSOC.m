function [numberof,depth]=rfofsoc(soc)
% function rfdemo2(ext)
%
% Basen on the Rainfall algorithm

error(nargchk(0,1,nargin))

if nargin==0,
    % turning points from 10000 random numbers
    ext=sig2ext(randn(10000,1));
else
    % turning points from vector ext
    [ext,exttime]=sig2ext(soc(:,2),soc(:,1));
end

rf=rainflow(ext,exttime);

%       rf(1,:) Cycles amplitude,
%       rf(2,:) Cycles mean value,
%       rf(3,:) Number of cycles (0.5 or 1.0),
%       rf(4,:) Begining time (when input includes dt or extt data),
%       rf(5,:) Cycle period (when input includes dt or extt data),

span=ceil(max(soc(:,2))-min(soc(:,2)))

figure, [numberof,depth]=rfhist(rf,span,'ampl')
figure, rfhist(rf,span,'mean')
figure, rfmatrix(rf,span,span)

% rfhist - histogram for use with rainflow data.
% function [no,xo] = rfhist(rf,x,rfflag)
% Syntax: rfhist(rf)
%         rfhist(rf,30)
%         rfhist(rf,-55:10:55,'mean')
%         [no,xo]=rfhist(rf,30);
% Input: rf     - rainflow data from rainflow function,
%                   see RAINFLOW for more details,
%        x      - when x is a scalar, uses x bins or when
%                   x is a vector, returns the distribution of rf 
%                   among bins with centers specified by x, like in hist(),
%        rfflag - string, data type flag,
%                   'ampl' for amplitude,
%                   'mean' for mean value,
%                   'freq' for frequency and 'period' for time period of
%                   extracted cycles.
% Output: no - vector, number of extracted cycles,
%         xo - vector, bin locations.