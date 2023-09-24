% Abre el archivo
fid = fopen('matrices.txt', 'r');

if fid == -1
    error('No se pudo abrir el archivo.');
end

% Lee las matrices desde el archivo
matrix_A = [];
matrix_B = [];
matrix_C = [];
current_matrix = '';

while ~feof(fid)
    line = fgetl(fid);
    if contains(line, 'Matriz A:')
        current_matrix = 'A';
    elseif contains(line, 'Matriz B:')
        current_matrix = 'B';
    elseif contains(line, 'Matriz C:')
        current_matrix = 'C';
    elseif ~isempty(line)
        values = sscanf(line, '%f');
        if current_matrix == 'A'
            matrix_A = [matrix_A; values'];
        elseif current_matrix == 'B'
            matrix_B = [matrix_B; values'];
        elseif current_matrix == 'C'
            matrix_C = [matrix_C; values'];
        end
    end
end

% Cierra el archivo
fclose(fid);

% Verifica si las matrices se pueden multiplicar
[mA, nA] = size(matrix_A);
[mB, nB] = size(matrix_B);

if nA ~= mB
    disp('Las matrices no se pueden multiplicar porque el número de columnas de A no es igual al número de filas de B.');
else
    % Realiza la multiplicación de matrices
    result_matrix = matrix_A * matrix_B;
    
    %disp('El resultado de la multiplicación de matrices es:');
    %disp(result_matrix);
    
    % Define la tolerancia para la comparación
    tolerancia = 1e-3;

    % Verifica si la diferencia entre los elementos es menor o igual a la tolerancia
    diferencia_dentro_de_tolerancia = all(abs(matrix_C - result_matrix) <= tolerancia);
    
    if diferencia_dentro_de_tolerancia
        disp('Las matrices son iguales dentro de la tolerancia especificada.');
    else
        disp('Las matrices no son iguales dentro de la tolerancia especificada.');
        % Calcula la matriz de diferencias entre matrix_C y result_matrix
        diferencias = abs(matrix_C - result_matrix);
        
        % Encuentra el valor máximo en la matriz de diferencias
        max_diferencia = max(diferencias(:));
        
        disp(['La mayor diferencia entre las celdas de las matrices es: ', num2str(max_diferencia)]);
    end
end