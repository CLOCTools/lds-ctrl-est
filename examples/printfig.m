function [] = printfig(fname, type, fig, size)
  % [] = printfig(fname, type, fig, size)
  %
  % type: type of figure ('pdf' [default]; 'svg'; 'eps'; 'tiff'; 'png'; 'jpg'; etc.)
  % fig: figure handle
  % size: [x, y] [=] inches
  %

  if ~exist('type', 'var')
    type = 'pdf';
  end

  if ~exist('fig','var')
    fig = gcf;
  end

  if nargin < 4
    size = [10 8]; %landscape 8x10 in by default
  end

  % Sets the size of figure to be printed.
  set(fig,'Units','Inches','PaperPosition',[0 0 size(1) size(2)])

  ext = type;
  if strcmp(type,'eps')
    type = [type 'c'];
  end

  if strcmp(type,'pdf')
     set(fig, 'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[size(1),size(2)])
  end

  % Use painters because will not rasterize.
  type_ = ['-d' type];
  if strcmp(type,'pdf')
    print(fig, [fname '.' ext], type_, '-painters', '-bestfit');
  else
    print(fig, [fname '.' ext], type_, '-painters');
  end
end
