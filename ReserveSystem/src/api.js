import axios from "axios";

class ReserveApi {
    static async reserveCar(carId){
        try{
            const body = {
                carID : carId,
            }
            const response = await axios.post(
                "",
                body
            )
            if(response) return response;

        }catch (e) {
            if(e.response) return e.response;
        }
    }
}

export default ReserveApi;
