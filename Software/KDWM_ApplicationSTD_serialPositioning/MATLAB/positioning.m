function calPos = positioning( pos, dis )

    posNum = size(pos, 1);
    pip1   = zeros(posNum, 2);	%  (xi - x1) , (yi - y1)
    pi2p12 = zeros(posNum, 3);	%  (xi^2 - x1^2) , (yi^2 - y1^2) , (di^2 - d1^2)
    for i = 2 : posNum
        pip1(i, 1)   = pos(i, 1) - pos(1, 1);
        pip1(i, 2)   = pos(i, 2) - pos(1, 2);
        pi2p12(i, 1) = pos(i, 1)^2 - pos(1, 1)^2;
        pi2p12(i, 2) = pos(i, 2)^2 - pos(1, 2)^2;
        pi2p12(i, 3) = dis(i)^2 - dis(1)^2;
    end

    matrixA = zeros(2, 2);
    matrixB = zeros(2, 1);
    for i = 2 : posNum
        matrixA(1, 1) = matrixA(1, 1) + pip1(i, 1)^2;               % sum (xi - x1)^2
        matrixA(1, 2) = matrixA(1, 2) + pip1(i, 1) * pip1(i, 2);	% sum (xi - x1) * (yi - y1)
        matrixA(2, 1) = matrixA(1, 2);                              % sum (xi - x1) * (yi - y1)
        matrixA(2, 2) = matrixA(2, 2) + pip1(i, 2)^2;               % sum (yi - y1)^2
        matrixB(1, 1) = matrixB(1, 1) + pip1(i, 1) * (pi2p12(i, 1) + pi2p12(i, 2) - pi2p12(i, 3));	% sum (xi - x1) * ( (xi^2 - x1^2) + (yi^2 - y1^2) - (di^2 - d1^2) )
        matrixB(2, 1) = matrixB(2, 1) + pip1(i, 2) * (pi2p12(i, 1) + pi2p12(i, 2) - pi2p12(i, 3));	% sum (yi - y1) * ( (xi^2 - x1^2) + (yi^2 - y1^2) - (di^2 - d1^2) )
    end

    if det(matrixA) == 0
        calPos = 0;
    else
        calPos = (inv(matrixA) * matrixB) * 0.5;
    end

end
